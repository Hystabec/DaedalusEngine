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
        private Rigidbody2DComponent m_rb;
        private FollowCamera m_camera;

        public float speed = 1.0f;

        void OnStart()
        {
            //Debug.Log($"Player.OnStart: {ID}");
            m_transform = Entity.Transform;
            //m_transform = Entity.GetComponent<TransformComponent>();

            m_rb = Entity.GetComponent<Rigidbody2DComponent>();
            m_camera = FindEntityByName("Camera").GetComponent<FollowCamera>();
        }

        void OnUpdate(float deltaTime)
        {
            //Debug.Log($"Player.OnUpdate: {deltaTime}");
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(InputCode.Key_W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(InputCode.Key_S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(InputCode.Key_A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(InputCode.Key_D))
                velocity.X = 1.0f;

            if (Input.IsKeyDown(InputCode.Key_Q))
                m_rb.ApplyTorque(1.0f);
            else if (Input.IsKeyDown(InputCode.Key_E))
                m_rb.ApplyTorque(-1.0f);

            velocity *= (speed);

            m_rb.ApplyForce(velocity.XY);

            if(m_camera)
            {
                if (Input.IsKeyDown(InputCode.Key_Z))
                    m_camera.distanceFromPlayer += 1.0f * deltaTime;
                else if (Input.IsKeyDown(InputCode.Key_X))
                    m_camera.distanceFromPlayer -= 1.0f * deltaTime;
            }

            //Vector3 position = m_transform.Position;
            //position += velocity;
            //m_transform.Position = position;
        }

    }
}
