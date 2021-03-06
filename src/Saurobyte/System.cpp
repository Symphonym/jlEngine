#include <Saurobyte/System.hpp>
#include <Saurobyte/Entity.hpp>
#include <Saurobyte/Engine.hpp>

namespace Saurobyte
{

	BaseSystem::BaseSystem(Engine *engineInstance, TypeID typeID)
		:
		MessageHandler(&engineInstance->getMessageCentral()),
		m_systemType(typeID),
		m_isActive(true),
		engine(engineInstance)
	{

	}

	void BaseSystem::addRequirement(const std::vector<TypeID> &componentIDs)
	{
		m_wantedEntities.push_back(componentIDs);
	}



	void BaseSystem::removeEntity(Entity &entity, bool wasKilled)
	{
		auto itr = m_monitoredEntities.find(entity.getID());
		if(itr != m_monitoredEntities.end())
		{
			m_monitoredEntities.erase(itr);

			if(wasKilled)
				onKill(entity);

			onDetach(entity);


		}
	}
	void BaseSystem::refreshEntity(Entity &entity)
	{
		auto itr = m_monitoredEntities.find(entity.getID());
		bool atleastOneMatch = false;

		for(std::size_t i = 0; i < m_wantedEntities.size(); i++)
		{
			bool matchingComponents = true;
			for(std::size_t c = 0; c < m_wantedEntities[i].size(); c++)
			{
				if(!entity.hasComponent(m_wantedEntities[i][c]))
				{
					matchingComponents = false;
					break;
				}
			}

			// A match was found and the entity is not already monitored
			if(matchingComponents)
			{
				atleastOneMatch = true;
				if(itr == m_monitoredEntities.end())
				{
					m_monitoredEntities[entity.getID()] = &entity;
					onAttach(entity);
					return;
				}
			}
		}

		// If the entity is monitored, but no component matches were found, stop monitoring it
		if(itr != m_monitoredEntities.end() && !atleastOneMatch)
		{
			m_monitoredEntities.erase(itr);
			onDetach(entity);
		}
	}

	void BaseSystem::processEntities()
	{
		for(auto itr = m_monitoredEntities.begin(); itr != m_monitoredEntities.end(); itr++)
		{
			if(itr->second->isActive())
				processEntity(*itr->second);
		}
	}

	void BaseSystem::setActive(bool active)
	{
		m_isActive = active;
	}

	void BaseSystem::clearSystem()
	{
		onClear();
		m_monitoredEntities.clear();
	}

	TypeID BaseSystem::getTypeID() const
	{
		return m_systemType;
	}
	const std::unordered_map<EntityID, Entity*>& BaseSystem::getEntities() const
	{
		return m_monitoredEntities;
	}
	bool BaseSystem::isActive() const
	{
		return m_isActive;
	}
};