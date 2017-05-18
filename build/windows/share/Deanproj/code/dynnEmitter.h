#ifndef DYNN_EMITTER_H
#define DYNN_EMITTER_H
#include <mutiny/mutiny.h>
#include "AntTweakBar.h"
#include "dynnParticles.h"

#include <vector>
#include <GL/glew.h>
#include "dynnRenderer.h"
;

namespace mutiny
{

	namespace engine
	{

	class GameObject;
	class Material;

		class dynnEmitter: public Component
		{
			friend class mutiny::engine::GameObject;
		public:
		
		private:
			ref<Mesh> particleShape;
			std::vector<ref<Mesh>> shapes;
			virtual void render();
			virtual void awake();
			int numbParticles;
			float centre;
			float radiusRange;
			Vector3 mouse;
			Vector3 position;
			Vector3 localPosition;
			TwBar *myBar;
			ref<Transform> transform;
			std::vector<dynnParticles> allParticles;
			GLuint vertBuff;
			GLuint uvBuff;
			ref<Material> material;
			Vector3 direction;
		
		};
	}
}
#endif