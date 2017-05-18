#ifndef DYNN_PARTICLES_H
#define DYNN_PARTICLES_H

#include <mutiny/mutiny.h>

//#include "Vector3.h"
//#include "Color.h"

namespace mutiny
{

	namespace engine
	{
		class dynnParticles
		{
		public:
			Vector3 pos;
			Vector3 vel;
			float scale;
			Vector3 colour;
			ref<GameObject> particleModel;
			float lifetime;
		};
		
		
	}
}

#endif