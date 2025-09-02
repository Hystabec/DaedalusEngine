using Daedalus;
using Daedalus.Types;

namespace Sandbox
{
    public class FollowCamera : MonoScript
    {
        public float distanceFromPlayer;

        private Entity followTarget;
        private TransformComponent m_transform;
        private TransformComponent m_targetTransform;

        void OnStart()
        {
            followTarget = FindEntityByName("Player");
            m_transform = Entity.Transform;

            if (followTarget)
                m_targetTransform = followTarget.GetComponent<TransformComponent>();
        }

        void OnUpdate(float deltaTime)
        {
            if(followTarget)
                m_transform.Position = new Vector3(m_targetTransform.Position.XY, distanceFromPlayer);
        }
    }
}
