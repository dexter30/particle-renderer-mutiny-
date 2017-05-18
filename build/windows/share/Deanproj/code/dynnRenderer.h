#ifndef DYNN_RENDERER_H
#define DYNN_RENDERER_H
#include <mutiny/mutiny.h>
#include "AntTweakBar.h"
#include <vector>

#include <GL/glew.h>

//#include "Vector3.h"
//#include "Color.h"
#include "dynnParticles.h"

namespace mutiny
{

	namespace engine
	{

	class GameObject;
	class Material;

		class dynnRenderer : public Component
		{
			friend class mutiny::engine::GameObject;
	public:
		void makeInterface(TwBar *bar);
		void makeInterface();
		void removeInterface();
		Vector3 getWorldPosition();
		static void drawParticles(int meshN);
		bool fin;
	private:
		ref<MeshRenderer> particleMod;
		ref<Mesh> desh;
		int numbParticles;
		float centre;
		float radiusRange;
		Vector3 mouse;
		Vector3 position;
		Vector3 localPosition;
		TwBar* thisBar;
		ref<Transform> transform;
		ref<Texture2d> cursor;
		Rect shape;
		std::vector<dynnParticles> allParticles;
		GLuint vertBuff;
		GLuint uvBuff;
		GLuint iconBuff;
		GLuint iconuvBuff;
		ref<Material> material;
		Vector3 direction;
		Vector3 colour1;
		virtual void render();
		virtual void awake();
		
		};
	
	}
	
}
#endif