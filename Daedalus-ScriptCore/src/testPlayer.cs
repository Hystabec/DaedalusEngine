using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Daedalus;
using Daedalus.Types;

namespace Sandbox
{
    /// <summary>
    /// Simple testing script
    /// </summary>
    public class PlayerScript : MonoScript
    {
        private TransformComponent m_transform;

        void OnStart()
        {
            //Debug.Log($"Player.OnStart: {ID}");
            m_transform = GetComponent<TransformComponent>();
        }

        void OnUpdate(float deltaTime)
        {
            //Debug.Log($"Player.OnUpdate: {deltaTime}");
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

            Vector3 position = m_transform.Position;
            position += velocity;
            m_transform.Position = position;
        }

    }
}
