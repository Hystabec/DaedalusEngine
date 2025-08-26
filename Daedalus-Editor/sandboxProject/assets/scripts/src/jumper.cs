﻿using Daedalus.Types;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sandbox
{
    public class Jumper : MonoScript
    {
        private Rigidbody2DComponent m_rb;

        public float jumpForce = 0.25f;
        public MonoScript other;

        private float m_time;

        void OnStart()
        {
            m_rb = GetComponent<Rigidbody2DComponent>();
        }

        void OnUpdate(float deltaTime)
        {
            m_time += deltaTime;
            if(m_time >= 1.0f)
            {
                m_time = 0.0f;
                m_rb.AddLinearImpulse(new Vector2(0.0f, jumpForce));
            }
        }

    }
}
