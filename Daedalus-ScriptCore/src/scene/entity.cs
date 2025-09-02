using System;

namespace Daedalus.Types
{
    // This class was added to differentiate a MonoScript (A script) and a Entity
    public class Entity
    {
        internal Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        // These allow if(Entity) to work
        // is an easy way to check if an entity is valid
        public static bool operator true(Entity x)
        {
            if(x != null)
                return x.ID != 0;
            else 
                return false;
        }
        public static bool operator false(Entity x)
        {
            if (x == null)
                return true;
            else
                return x.ID == 0;
        }
        public static bool operator !(Entity x)
        {
            if (x == null)
                return true;
            else
                return x.ID == 0;
        }

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
            return InternalCalls.entity_has_component(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (typeof(T).IsSubclassOf(typeof(MonoScript)))
            {
                object instance = InternalCalls.entity_get_script_instance(ID);
                return instance as T;
            }
            else 
            {
                if (!HasComponent<T>())
                    return null;

                T component = new T() { Entity = this };
                return component;
            }
        }

    }
}
