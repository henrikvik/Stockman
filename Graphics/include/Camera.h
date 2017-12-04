#pragma once

#include "Utility\ConstantBuffer.h"
#include "RenderInfo.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <SimpleMath.h>

namespace Graphics {
	class Camera
	{
	public:
		struct ShaderValues {
			DirectX::SimpleMath::Matrix vP;
			DirectX::SimpleMath::Matrix invP;
			DirectX::SimpleMath::Matrix view;
			DirectX::SimpleMath::Matrix shadow;
            DirectX::SimpleMath::Vector4 camPos;
            DirectX::SimpleMath::Vector4 forward;
		} values;

		struct InverseMatrixes
		{
			DirectX::SimpleMath::Matrix invView;
			DirectX::SimpleMath::Matrix invP;
		};


		Camera(ID3D11Device* device, int width, int height, float drawDistance = 100, float fieldOfView = M_PI * 0.45);
		~Camera();

		DirectX::SimpleMath::Vector3 getPos() const;
		DirectX::SimpleMath::Vector3 getForward() const;
		DirectX::SimpleMath::Matrix getView() const;
		DirectX::SimpleMath::Matrix getProj() const;
		float getFarPlane() const { return far_distance; };
		ConstantBuffer<ShaderValues>* getBuffer();
		ConstantBuffer<InverseMatrixes>* getInverseBuffer();

		void update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context);
        void updateLookAt(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, ID3D11DeviceContext* context);
        void setShadowDir(DirectX::SimpleMath::Matrix dir) { values.shadow = dir; }

        bool inside_frustrum(DirectX::SimpleMath::Vector3 point, float radius = 1.0f);
        void render() const;
        void updateFOV(float fieldOfView);
	private:
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 forward;

        float far_distance;
        float near_distance;
		float field_of_view;
        float aspect_ratio;

		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;

		InverseMatrixes inverseMatrixes;

        using Vector3 = DirectX::SimpleMath::Vector3;
        using Matrix = DirectX::SimpleMath::Matrix;
        struct Plane
        {
            Plane() {};
            Plane(Vector3 normal, float d, Vector3 up = {0,1,0})
            {
                normal.Normalize();
                up.Normalize();

                this->normal = normal;
                Vector3 left = normal.Cross(up);
                top_left  =  left + up + normal * d;
                top_right = -left + up + normal * d;
                bot_left  =  left - up + normal * d;
                bot_right = -left - up + normal * d;
            }

            Vector3 normal;
            Vector3 top_left;
            Vector3 top_right;
            Vector3 bot_left;
            Vector3 bot_right;

            void transform(Matrix & matrix)
            {
                top_left  = DirectX::XMVector3TransformCoord(top_left , matrix);
                top_right = DirectX::XMVector3TransformCoord(top_right, matrix);
                bot_left  = DirectX::XMVector3TransformCoord(bot_left , matrix);
                bot_right = DirectX::XMVector3TransformCoord(bot_right, matrix);

                normal = (top_right - top_left).Cross(bot_left - top_left);
                normal.Normalize();
            }

            float distance(Vector3 point)
            {
                return normal.Dot(point - top_left);
            }
        };

        Plane frustrum_planes[6];
        void calc_frustrum_planes();

		ConstantBuffer<ShaderValues> cameraBuffer;
		ConstantBuffer<InverseMatrixes> inverseBuffer;

        std::vector<NewDebugRenderInfo::Point> debug_points;
        NewDebugRenderInfo debug_info;
	};
}