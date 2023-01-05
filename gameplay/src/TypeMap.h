#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace gameplay
{
	// @TODO Implement thread-safeness
	// 
	/**
	 * @brief A map class assigns types of classes with values
	 * It can have only one class type for instances
	 *
	 * Based on:https://stackoverflow.com/a/9859605
	 *
	 * Example usage:
	 *
	 * class Component
	 * {
	 *	//...
	 * }
	 *
	 * class Entity
	 * {
	 *	//...
	 *	TypeMap<Component> components;
	 * }
	 *
	 * class MoveComponent : public Component
	 * {
	 *	//...
	 * }
	 *
	 * class CollisionComponent : public Component
	 * {
	 *	//...
	 * }
	 *
	 * Entity entity;
	 * entity.components.add<MoveComponent>(new MoveComponent());
	 * entity.components.add<CollisionComponent>(new CollisionComponent());
	 *
	 * CollisionComponent *collisionComponent = entity.components.get<CollisionComponent>();
	 *
	 * @tparam Value Base value data type
	 */
	template <typename Value>
	class TypeMap
	{
	public:

		typedef typename std::unordered_map<std::type_index, Value*> Map;
		typedef typename Map::iterator Iterator;

		TypeMap()
		{

		}

		virtual ~TypeMap()
		{
			clear();
		}

		/**
		 * @brief Adds a value to the map whic assigned with Key
		 * Does not add if the same Key class type exists
		 *
		 * @tparam Key Key class type
		 * @param value Value will be added to the map
		*/
		template<typename Key>
		void add(Value* value)
		{
			if (has<Key>())
			{
				return;
			}
			map[typeid(Key)] = value;
		}

		/**
		 * @brief
		 * get value as Type which assigned by Type
		 * @tparam Type
		 * @tparam std::enable_if<std::is_base_of<ValueType, Type>::value>::type
		 * @return Type*
		 */
		template<typename Key, typename ReturnType = Key,
			typename std::enable_if<std::is_base_of<Value, ReturnType>::value>::type* = nullptr>
			ReturnType* get()
		{
			if (map.find(typeid(Key)) != map.end())
			{
				return static_cast<ReturnType*>(map[typeid(Key)]);
			}
			return nullptr;
		}

		/**
		 * @brief Removes if Key type exists
		 *
		 * @tparam Key Key class type which will be quiered
		 */
		template<typename Key>
		void remove()
		{
			// Does not remove if not exists
			if (hasNot<Key>())
			{
				return;
			}

			Value* value = get<Key>();
			if (value)
			{
				delete value;
				value = nullptr;
			}
			map.erase(typeid(Key));
		}

		/**
		 * @brief Checks the map has Key
		 *
		 * @tparam Key Key class type which will be quiered
		 * @return true Returns true if has Key type
		 * @return false Returns false if has not Key type
		 */
		template<typename Key>
		bool has()
		{
			Iterator it = map.find(typeid(Key));
			if (it != map.end())
			{
				return true;
			}
			return false;
		}

		/**
		 * @brief Checks the map has not Key type
		 *
		 * @tparam Key Key class type which will be quiered
		 * @return true Returns true if has not Key type
		 * @return false Returns false if has Key type
		 */
		template<typename Key>
		bool hasNot()
		{
			return (!has<Key>());
		}

		/**
		 * @brief Begin iterator. Useful for iterating
		 *
		 * @return Iterator
		 */
		Iterator begin()
		{
			return map.begin();
		}

		/**
		 * @brief End iterator. Useful for iterating
		 *
		 * @return Iterator
		 */
		Iterator end()
		{
			return map.end();
		}

		/**
		 * @brief Clears all elements
		 *
		 */
		void clear()
		{
			for (Iterator it = map.begin(); it != map.end(); ++it)
			{
				if (it->second)
				{
					delete it->second;
					it->second = nullptr;
				}
			}
			map.clear();
		}

	private:
		Map map;
	};
}

#endif