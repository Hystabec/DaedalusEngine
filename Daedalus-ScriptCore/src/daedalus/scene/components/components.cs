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
        public MonoScript MonoScript {  get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.transform_component_get_position(MonoScript.EntityID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.transform_component_set_position(MonoScript.EntityID, ref value);
            }
        }
    }

}
