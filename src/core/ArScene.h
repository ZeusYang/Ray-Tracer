#ifndef ARSCENE_H
#define ARSCENE_H

#include "ArAurora.h"
#include "ArMathUtils.h"
#include "ArLight.h"
#include "ArHitable.h"

#include <vector>

namespace Aurora
{
	class AScene final
	{
	public:
		typedef std::shared_ptr<AScene> ptr;

		AScene(const AHitableAggregate::ptr &hitables, const std::vector<ALight::ptr> &lights) 
			: m_lights(lights), m_aggreShape(hitables)
		{
			m_worldBound = m_aggreShape->worldBound();
			for (const auto &light : lights)
			{
				light->preprocess(*this);
				if (light->flags & (int)ALightFlags::ALightInfinite)
					m_infiniteLights.push_back(light);
			}
		}

		const ABounds3f &worldBound() const { return m_worldBound; }

		bool hit(const ARay &ray) const;
		bool hit(const ARay &ray, ASurfaceInteraction &isect) const;
		bool hitTr(ARay ray, ASampler &sampler, ASurfaceInteraction &isect, ASpectrum &transmittance) const;

		std::vector<ALight::ptr> m_lights;
		// Store infinite light sources separately for cases where we only want
		// to loop over them.
		std::vector<ALight::ptr> m_infiniteLights;

	private:
		// Scene Private Data
		ABounds3f m_worldBound;
		AHitableAggregate::ptr m_aggreShape;
	};
}

#endif