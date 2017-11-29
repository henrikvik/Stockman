#include <Graphics\include\Camera.h>
#include "RenderQueue.h"
#include <Keyboard.h>
#include <iterator>

namespace Graphics
{
    Camera::Camera(ID3D11Device* device, int width, int height, float drawDistance, float fieldOfView) :
        cameraBuffer(device),
        inverseBuffer(device)
    {
        this->near_distance = 0.1f;
        float aspect_ratio = float(width) / height;

        this->fieldOfView = fieldOfView;
        this->far_distance = drawDistance;

        this->projection = DirectX::XMMatrixPerspectiveFovRH(fieldOfView, aspect_ratio, near_distance, far_distance);

        values.vP = this->projection * this->view;
        values.view = this->view;
        values.invP = this->projection.Invert();

        float fovtan = 2 * tan(fieldOfView * 0.5f);
        near_height = fovtan * near_distance;
        near_width = near_height * aspect_ratio;
        far_height = fovtan * far_distance;
        far_width = far_height * aspect_ratio;
    }

    Camera::~Camera()
    {

    }

    void Camera::update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context)
    {
        forward.Normalize();
        DirectX::SimpleMath::Vector3 local_up(0, 1, 0);
        this->view = DirectX::XMMatrixLookToRH(pos, forward, local_up);

        this->position = pos;
        this->forward = view.Forward();
        this->right = view.Right();
        this->up = view.Up();

        values.view = this->view;
        values.vP = this->view * this->projection;
        values.invP = this->projection.Invert();
        values.camPos = DirectX::SimpleMath::Vector4(pos.x, pos.y, pos.z, 1);
        values.forward = DirectX::SimpleMath::Vector4(forward.x, forward.y, forward.z, 0);

        inverseMatrixes.invP = values.invP;
        inverseMatrixes.invView = view.Invert();

        cameraBuffer.write(context, &values, sizeof(ShaderValues));
        inverseBuffer.write(context, &inverseMatrixes, sizeof(InverseMatrixes));

        debug_info.points = &debug_points;
        debug_info.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        debug_info.useDepth = true;

        calc_frustrum_planes();
    }

    void Camera::updateLookAt(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, ID3D11DeviceContext * context)
    {
        Camera::update(pos, target - pos, context);
    }

    bool Camera::inside_frustrum(DirectX::SimpleMath::Vector3 point, float radius)
    {
        bool inside_frustrum = true;

        for (size_t i = 0; i < 6 && inside_frustrum; i++)
        {
            inside_frustrum = frustrum_planes[i].distance(point) + radius > 0;
        }

        return inside_frustrum;
    }

    void Camera::render() const
    {
        QueueRender(debug_info);
    }

    void Camera::calc_frustrum_planes()
    {
        static auto ks = DirectX::Keyboard::KeyboardStateTracker();
        ks.Update(DirectX::Keyboard::Get().GetState());
        static bool update_frustrum = false;
        static bool o_was_pressed = false;
        update_frustrum = ks.pressed.O && !o_was_pressed ? !update_frustrum : update_frustrum;
        o_was_pressed = ks.pressed.O;

        if (update_frustrum)
        {
            using namespace DirectX::SimpleMath;            
            enum Planes { NEAR_, FAR_, LEFT_, RIGHT_, TOP_, BOT_ };
            frustrum_planes[NEAR_]  = {{ 0, 0, 1}, -1};
            frustrum_planes[FAR_]   = {{ 0, 0,-1}, -1};
            frustrum_planes[LEFT_]  = {{ 1, 0, 0}, -1};
            frustrum_planes[RIGHT_] = {{-1, 0, 0}, -1};
            frustrum_planes[TOP_]   = {{ 0,-1, 0}, -1, { 0, 0, 1}};
            frustrum_planes[BOT_]   = {{ 0, 1, 0}, -1, { 0, 0, 1}};
                                               
            Matrix ndc_to_world = projection.Invert() * view.Invert();
            for (auto & plane : frustrum_planes)
            {
                plane.transform(ndc_to_world);
            }

            #ifdef _DEBUG
            auto draw_square = [&](Vector3 top_left, Vector3 top_right, Vector3 bot_left, Vector3 bot_right, Color color)
            {
                debug_points.emplace_back(top_left, color);
                debug_points.emplace_back(top_right, color);
                debug_points.emplace_back(bot_left, color);
                debug_points.emplace_back(bot_right, color);
                debug_points.emplace_back(top_left, color);
                debug_points.emplace_back(bot_left, color);
                debug_points.emplace_back(top_right, color);
                debug_points.emplace_back(bot_right, color);

                Vector3 top_half = (top_left + top_right)    * 0.5f;
                Vector3 bot_half = (bot_left + bot_right)    * 0.5f;
                Vector3 left_half = (top_left + bot_left)    * 0.5f;
                Vector3 right_half = (top_right + bot_right) * 0.5f;
            };
            auto draw_plane = [&](Plane & plane, Color color)
            {
                Vector3 offset = -plane.normal * 0.05f;
                draw_square(plane.top_left + offset, plane.top_right + offset, plane.bot_left + offset, plane.bot_right + offset, color);

                debug_points.emplace_back(position, color);
                debug_points.emplace_back(position + plane.normal, color);
            };
            Color colors[] = {{0, 0, 1},{0, 1, 0},{0, 1, 1},{1, 0, 0},{1, 0, 1},{1, 1, 0}};
            debug_points.clear();
            for (size_t i = 0; i < 6; i++)
            {
                draw_plane(frustrum_planes[i], colors[i]);
            }
            #endif
        }
    }

    DirectX::SimpleMath::Vector3 Camera::getPos() const
    {
        return this->position;
    }

    DirectX::SimpleMath::Vector3 Camera::getForward() const
    {
        return this->forward;
    }

    DirectX::SimpleMath::Vector3 Camera::getUp() const
    {
        return this->view.Up();
    }

    DirectX::SimpleMath::Vector3 Camera::getRight() const
    {
        return this->view.Right();
    }

    DirectX::SimpleMath::Matrix Camera::getView() const
    {
        return view;
    }

    DirectX::SimpleMath::Matrix Camera::getProj() const
    {
        return projection;
    }

    ConstantBuffer<Camera::ShaderValues>* Camera::getBuffer()
    {
        return &this->cameraBuffer;
    }

    ConstantBuffer<Camera::InverseMatrixes>* Camera::getInverseBuffer()
    {
        return &this->inverseBuffer;
    }

}