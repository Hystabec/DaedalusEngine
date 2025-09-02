using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// NOTE: this file might want to be split into seperate files, if it gets to big
namespace Daedalus.Types
{
    public abstract class Component
    {
        public Entity Entity {  get; internal set; }

        public static bool operator true(Component x)
        {
            if (x != null)
                return x.Entity.ID != 0;
            else
                return false;
        }
        public static bool operator false(Component x)
        {
            if (x == null)
                return true;
            else
                return x.Entity.ID == 0;
        }
        public static bool operator !(Component x)
        {
            if (x == null)
                return true;
            else
                return x.Entity.ID == 0;
        }
    }

    public class TransformComponent : Component
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.transform_component_get_position(Entity.ID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.transform_component_set_position(Entity.ID, ref value);
            }
        }
    }

    public class Rigidbody2DComponent : Component
    {
        public void ApplyForce(Vector2 force, Vector2 worldPoint, bool wake = true)
        {
            InternalCalls.rigidbody2D_component_apply_force_from_point(Entity.ID, ref force, ref worldPoint, wake);
        }

        public void ApplyForce(Vector2 force, bool wake = true)
        {
            InternalCalls.rigidbody2D_component_apply_force(Entity.ID, ref force, wake);
        }

        public void AddLinearImpulse(Vector2 linearImpulse, Vector2 worldPoint, bool wake = true)
        {
            InternalCalls.rigidbody2D_component_add_linear_impulse_from_point(Entity.ID, ref linearImpulse, ref worldPoint, wake);
        }

        public void AddLinearImpulse(Vector2 linearImpulse, bool wake = true)
        {
            InternalCalls.rigidbody2D_component_add_linear_impulse(Entity.ID, ref linearImpulse, wake);
        }

        public void ApplyTorque(float torque, bool wake = true)
        {
            InternalCalls.rigidbody2D_component_apply_torque(Entity.ID, ref torque, wake);
        }
    }

}
