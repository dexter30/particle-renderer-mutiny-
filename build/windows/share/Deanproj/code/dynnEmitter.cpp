#include "dynnEmitter.h"
namespace mutiny
{

	namespace engine
	{
		void dynnEmitter::awake()
		{
			glGenBuffers(1, &vertBuff);
			glGenBuffers(1, &uvBuff);
			particleShape = Resources::load<Mesh>("models/curuthers/curuthers");
			shapes.push_back(particleShape);
			std::vector<float> valuesToGPU; //vertex coords to be uploaded to buffers
			for(int i = 0; i < particleShape->getVertices().size(); i++)
			{
				valuesToGPU.push_back(particleShape->getVertices().at(i).x);
				valuesToGPU.push_back(particleShape->getVertices().at(i).y);
				valuesToGPU.push_back(particleShape->getVertices().at(i).z);
			}
			glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
			glBufferData(GL_ARRAY_BUFFER, valuesToGPU.size() * sizeof(valuesToGPU[0]), &valuesToGPU[0], GL_STATIC_DRAW);
			valuesToGPU.clear();
			
			
			
			std::vector<Vector2> uv;

			uv.push_back(Vector2(0, 1));
			uv.push_back(Vector2(0, 0));
			uv.push_back(Vector2(1, 0));
			uv.push_back(Vector2(1, 0));
			uv.push_back(Vector2(1, 1));
			uv.push_back(Vector2(0, 1));
  
			for(int i = 0; i < uv.size(); i++)
			{
				valuesToGPU.push_back(uv.at(i).x);
				valuesToGPU.push_back(uv.at(i).y);
			}  
			glBindBuffer(GL_ARRAY_BUFFER, uvBuff);
			glBufferData(GL_ARRAY_BUFFER, valuesToGPU.size() * sizeof(valuesToGPU[0]), &valuesToGPU[0], GL_STATIC_DRAW);
				radiusRange = 5.0f;
			for(int i = 0; i < 25; i++)
			{
				dynnParticles part1;
				part1.pos = Vector3(Random::range(-0.5f, 0.5f), Random::range(-0.5f, 0.5f), Random::range(-0.5f, 0.5f));
				part1.vel = Vector3(-0.6, 0.2f, 0);
				part1.lifetime = Random::range(1.0f, radiusRange);
				allParticles.push_back(part1);
			}
			
			direction = Vector3(1.0f,1.0f,1.0f);
			centre = 0.0f;
		
			numbParticles = allParticles.size();
			localPosition = Vector3(0.0f,0.0f,0.0f);
			transform = getGameObject()->getTransform();
		}
		
		void dynnEmitter::render()
		{
			ref<Shader> shader;
			
			material = Resources::load<Material>("shaders/particle");
			
			shader = material->getShader();
			glUseProgram(shader->programId->getGLuint());
			if(material.expired())
			{
				std::cout<<"somethings up m8";
			}
			
			//std::cout<<particleShape->getVertices()[0].x;
			Vector3 axisMod;
			Matrix4x4 viewMat = Matrix4x4::getIdentity();
			axisMod = Camera::getCurrent()->getGameObject()->getTransform()->getRotation();
			axisMod.x *= -1;
			axisMod.y *= -1;
			viewMat = viewMat.rotate(axisMod);
			axisMod = Camera::getCurrent()->getGameObject()->getTransform()->getPosition();
			axisMod.x *= -1;
			axisMod.y *= -1;
			viewMat = viewMat.translate(axisMod);
			
			material->setMatrix("in_Projection", Camera::getCurrent()->getProjectionMatrix());
			material->setMatrix("in_View", viewMat);
			
			GLint positionAttribId = glGetAttribLocation(shader->programId->getGLuint(), "in_Position");
			
			glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
			glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribId);
			
			GLint uvAttribId = glGetAttribLocation(shader->programId->getGLuint(), "in_Uv");
			
			glBindBuffer(GL_ARRAY_BUFFER, uvBuff);
			glVertexAttribPointer(uvAttribId, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(uvAttribId);
			
			GLuint uniformId = glGetUniformLocation(shader->programId->getGLuint(),"in_Projection");
			glUniformMatrix4fv(uniformId, 1, GL_FALSE, Camera::getCurrent()->getProjectionMatrix().getValue());
			
			GLuint uniformId2 = glGetUniformLocation(shader->programId->getGLuint(),"in_View");
			glUniformMatrix4fv(uniformId2, 1, GL_FALSE, viewMat.getValue());
			position = transform->getPosition();
			glDisable(GL_DEPTH_TEST);
			for(size_t i = 0; i < allParticles.size(); i++)
			{
				
				Matrix4x4 modelMat = Matrix4x4::getIdentity();
				axisMod = allParticles.at(i).pos ;
				axisMod.z *= -1;
				modelMat = modelMat.translate(axisMod);
				modelMat = modelMat* modelMat.rotate(Vector3(0.0f, 0, 180.0f));
				//modelMat = modelMat.scale(Vector3(3.0f,3.0f,3.0f));
				modelMat = modelMat.scale(Vector3(0.5f,0.5f,0.5f));
				material->setMatrix("in_Model", modelMat);
				
				GLuint uniformId3 = glGetUniformLocation(shader->programId->getGLuint(),"in_Model");
				glUniformMatrix4fv(uniformId3, 1, GL_FALSE, modelMat.getValue());
				
				//glDrawArrays(GL_TRIANGLES, 0, particleShape->getVertices().size()* 3);
				//dynnRenderer::drawParticles(particleShape->getVertices().size()* 3);
				
				glDrawArrays(GL_TRIANGLES, 0, particleShape->getVertices().size()* 3);
				std::cout<<i<<": "<<allParticles.at(i).pos.x<<"\n";
				if (allParticles.at(i).lifetime <= 0)
				{
					
					Matrix4x4 rotat = Matrix4x4::getIdentity();

					//fix this rotation currently not rotating around object which is wrong
				
					rotat = rotat * rotat.rotate(Vector3(Random::range(centre -radiusRange,centre + radiusRange), Random::range(centre -radiusRange,centre + radiusRange), Random::range(centre -radiusRange,centre + radiusRange)));
										
					allParticles.at(i).pos = transform->getPosition() + localPosition;
										
					
					allParticles.at(i).lifetime = Random::range(1.0f,5.0f) ;
					
					allParticles.at(i).vel = rotat * direction;
					
				}
				allParticles.at(i).pos = allParticles.at(i).pos + (allParticles.at(i).vel * Time::getDeltaTime());
				allParticles.at(i).lifetime -= Time::getDeltaTime();
				
				glEnable(GL_DEPTH_TEST);
				glDisableVertexAttribArray(positionAttribId);
				
				glDisableVertexAttribArray(uvAttribId);
			}
		}
	}
}