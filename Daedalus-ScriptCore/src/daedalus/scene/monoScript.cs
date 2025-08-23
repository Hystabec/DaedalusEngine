using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daedalus.Types
{
    // NOTE: This was originally called 'Entity' then renamed to 'MonoScript'
    // would something like 'Componet'/'BaseComponent' be better?
    public class MonoScript
    {
        protected MonoScript()
        {
            EntityID = 0;
        }

        internal MonoScript(ulong id)
        {
            EntityID = id;
        }

        public readonly ulong EntityID;

        public TransformComponent Transform
        {
            get
            {
                TransformComponent tc = GetComponent<TransformComponent>();
                return tc;
            }
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.entity_has_component(EntityID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if(!HasComponent<T>())
                return null;

            T component = new T() { MonoScript = this };
            return component;
        }
    }
}
