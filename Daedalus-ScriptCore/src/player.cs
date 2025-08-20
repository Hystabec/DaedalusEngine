using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Daedalus;

namespace Sandbox
{
    /// <summary>
    /// Simple testing script
    /// </summary>
    public class Player : Entity
    {
        void OnStart()
        {
            //InternalCalls.native_log($"Player.OnStart: {ID}");
        }

        void OnUpdate(float deltaTime)
        {
            //InternalCalls.native_log($"Player.OnUpdate: {deltaTime}");

            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(InputCode.Key_W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(InputCode.Key_S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(InputCode.Key_A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(InputCode.Key_D))
                velocity.X = 1.0f;

            velocity *= (speed * deltaTime);

            Vector3 position = Position;
            position += velocity;
            Position = position;
        }

    }
}
