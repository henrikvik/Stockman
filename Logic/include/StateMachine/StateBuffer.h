#ifndef STATEBUFFER_H
#define STATEBUFFER_H

#pragma region Comment

//   For communicating between MenuStates and GameStates
//
//      It was either this way or a actual buffer system (See last version of this class)
//          This is worse in terms of coding but easier to understand and write
//          Just grab the state from the other side and dynamic_cast it. 
//          Don't forget to check for nullptr after casting.
//          Don't kill me Mats-Ola.

#pragma endregion Description of class

namespace Logic
{
    class State;
    struct StateBuffer
    {
        StateBuffer() : currentPrimaryState(nullptr), currentSecondaryState(nullptr) { }
        State* currentPrimaryState;
        State* currentSecondaryState;
    };
}

#endif // !STATEBUFFER_H