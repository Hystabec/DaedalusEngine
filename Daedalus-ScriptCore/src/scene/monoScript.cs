using System;

namespace Daedalus.Types
{
    // NOTE: This was originally called 'Entity' then renamed to 'MonoScript'
    // would something like 'Componet'/'BaseComponent' be better?
    public class MonoScript : Component
    {
        protected MonoScript()
        {
            Entity = new Entity(0);
        }

        internal MonoScript(ulong id)
        {
            Entity = new Entity(id);
        }

        //public readonly Entity Entity;

        // Wrappers for entity functions
        // so 'Transform' can be used instead of 'Entity.Transform'
        //public TransformComponent Transform
        //{
        //    get
        //    {
        //        return Entity.GetComponent<TransformComponent>();
        //    }
        //}

        //public bool HasComponent<T>() where T : Component, new()
        //{
        //    return Entity.HasComponent<T>();
        //}

        //public T GetComponent<T>() where T : Component, new()
        //{
        //    return Entity.GetComponent<T>();
        //}

        public Entity FindEntityByName(string name)
        {
            ulong id = InternalCalls.entity_find_entity_by_name(name);

            if(id == 0)
            {
                return null;
            }

            return new Entity(id);
        }
    }
}
