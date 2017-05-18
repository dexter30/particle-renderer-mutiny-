#include "dynnRenderer.h"
#include <iostream>

namespace mutiny
{

	namespace engine
	{
		void dynnRenderer::awake()
		{
			desh  = Resources::load<Mesh>("models/square");
			cursor = Resources::load<Texture2d>("text/source");
		
			shape.width = 10.0f;
			shape.height = 10.0f;
			
			
			//myBar = TwNewBar("NameOfMyTweakBar");
			transform = getGameObject()->getTransform();
			localPosition = Vector3(0.0f,0.0f,0.0f);
			 
			glGenBuffers(1, &vertBuff);
			glGenBuffers(1, &uvBuff);
			


			
			                                    
			

			
			std::vector<float> valuesToGPU; //vertex coords to be uploaded to buffers

			
			for(int i = 0; i < desh->getVertices().size(); i++)
			{
				valuesToGPU.push_back(desh->getVertices().at(i).x);
				valuesToGPU.push_back(desh->getVertices().at(i).y);
				valuesToGPU.push_back(desh->getVertices().at(i).z);
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
			
			direction = Vector3(1.0f,1.0f,1.0f);
			centre = 0.0f;
			radiusRange = 10.0f;
			//TwAddVarRW(myBar, "Direction", TW_TYPE_DIR3F, &direction, " axisz=-z ");
			//TwAddVarRW(myBar, "Number Particles", TW_TYPE_INT32, &numbParticles ," label='Number of Particles' keyIncr=c keyDecr=C help='Defines the number of parts in the scene.' ");
			//TwAddVarRW(myBar, "Radius", TW_TYPE_FLOAT, &radiusRange,"min=0 max=180"); 
			//TwAddVarRW(myBar, "Source.x", TW_TYPE_FLOAT, &localPosition.x,"group='Position'  step=0.01 ");
			//TwAddVarRW(myBar, "Source.y", TW_TYPE_FLOAT, &localPosition.y,"group='Position'  step=0.01 ");
			//TwAddVarRW(myBar, "Source.z", TW_TYPE_FLOAT, &localPosition.z,"group='Position'  step=0.01 ");
			//TwAddVarRW(myBar, "Color", TW_TYPE_COLOR3F, &colour1, " label='Particle color' ");
			//TwAddButton(myBar, "Run", addParticles, NULL, " label='Run Forest' ");
			numbParticles = 5;
			colour1.x = 1.0f;
			colour1.y = 0.0f;
			colour1.z = 0.0f;
			fin =false;
		}
		
		void dynnRenderer::render()
		{
			position = transform->getPosition();
	
			
		
			
			
		
			ref<Shader> shader;
			
			material = Resources::load<Material>("shaders/particle");
			
			shader = material->getShader();
			glUseProgram(shader->programId->getGLuint());
			if(material.expired())
			{
				std::cout<<"somethings up m8";
			}
			
			
		
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
			
		
			
			
			GLuint uniformColour = glGetUniformLocation(shader->programId->getGLuint(),"in_Colour");
			glUniform3f(uniformColour, colour1.x, colour1.y, colour1.z);
			
			mouse.x = Input::getMousePosition().x;
			mouse.y = Input::getMousePosition().y;
			
			
			glDisable(GL_DEPTH_TEST);
						
	
			//glFrontFace(GL_CW);
			for(size_t i = 0; i < allParticles.size(); i++)
			{
				
				//std::cout<<i<<": "<<allParticles.at(i).pos.x<<" "<<allParticles.at(i).pos.y<<" "<<allParticles.at(i).pos.z<<"\n";
				//std::cout<<transform->getPosition().x<<" "<<transform->getPosition().y<<" "<<transform->getPosition().z;
				Matrix4x4 modelMat = Matrix4x4::getIdentity();
				axisMod = allParticles.at(i).pos;
				axisMod.z *= -1;
				modelMat = modelMat.translate(axisMod);
				modelMat = modelMat* modelMat.rotate(Vector3(0.0f, 0, 180.0f));
				//modelMat = modelMat.scale(Vector3(3.0f,3.0f,3.0f));
				modelMat = modelMat.scale(Vector3(0.10f,0.10f,0.10f));
				material->setMatrix("in_Model", modelMat);
				
				
		
				
				
				
				GLuint uniformId3 = glGetUniformLocation(shader->programId->getGLuint(),"in_Model");
				glUniformMatrix4fv(uniformId3, 1, GL_FALSE, modelMat.getValue());

				glDrawArrays(GL_TRIANGLES, 0, desh->getVertices().size()* 3);
				
				
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
				
				
				
			}			
			 
			if(Input::getKey(KeyCode::LEFT ) == true || allParticles.size() > numbParticles)
			{
				
				if (allParticles.size() >=1)
				{
				allParticles.erase(allParticles.begin());
				}
				numbParticles = allParticles.size();
				
			}
			if(Input::getKey(KeyCode::RIGHT) == true || allParticles.size() < numbParticles)
			{
				//TwAddVarRO(myBar, "Position.x", TW_TYPE_FLOAT, &position.x,"group='Position'");
				//TwAddVarRO(myBar, "Position.y", TW_TYPE_FLOAT, &position.y,"group='Position'");
				dynnParticles newParticle;                
				newParticle.pos = transform->getPosition();
				//newParticle.vel = Vector3(Random::range(-radiusRange,radiusRange), Random::range(-radiusRange,radiusRange),Random::range(-radiusRange,radiusRange));
				newParticle.vel = direction;
				allParticles.push_back(newParticle);
				numbParticles = allParticles.size();
			
			}
			if(fin)
			{
				removeInterface();
			}
			else
			{
				
			}
			
			
			
			getWorldPosition();
			 glEnable(GL_DEPTH_TEST);
			 
			 
			 
			
			
			glDisableVertexAttribArray(positionAttribId);
			
			glDisableVertexAttribArray(uvAttribId);
			
		}
		
		Vector3 dynnRenderer::getWorldPosition()
		{
			Vector3 boo;
			boo = Input::getMousePosition();
			boo.x = boo.x - (Screen::getWidth() / 2.0);
			boo.y = -(boo.y - (Screen::getHeight() / 2.0));
			Input::getMousePosition();
			Matrix4x4 mat = (Camera::getCurrent()->getProjectionMatrix() *Matrix4x4::getTrs(
			Camera::getCurrent()->getGameObject()->getTransform()->getPosition(),
			Camera::getCurrent()->getGameObject()->getTransform()->getRotation(),
			Vector3(1, 1, 1) * Vector3(1, 1, -1)
			)).inverse();
			Vector3 myBoo = mat * boo;
			//myBoo.y = myBoo;
			//std::cout<<boo.x* 0.03<<","<<boo.y * 0.03<<"\n";
	
			
			return myBoo;
		}
		void dynnRenderer::drawParticles(int meshN)
		{
			glDrawArrays(GL_TRIANGLES, 0, meshN);
		}
		
		void dynnRenderer::makeInterface(TwBar *bar)
		{
			TwRemoveAllVars(bar);
			thisBar = bar;
			TwAddVarRW(bar, "Direction", TW_TYPE_DIR3F, &direction, "opened=true axisz=-z ");
			TwAddVarRW(bar, "Number Particles", TW_TYPE_INT32, &numbParticles ," label='Number of Particles' keyIncr=c keyDecr=C help='Defines the number of parts in the scene.' ");
			TwAddVarRW(bar, "Radius", TW_TYPE_FLOAT, &radiusRange,"min=0 max=180"); 
			TwAddVarRW(bar, "Source.x", TW_TYPE_FLOAT, &localPosition.x,"group='Position'  step=0.01 ");
			TwAddVarRW(bar, "Source.y", TW_TYPE_FLOAT, &localPosition.y,"group='Position'  step=0.01 ");
			TwAddVarRW(bar, "Source.z", TW_TYPE_FLOAT, &localPosition.z,"group='Position'  step=0.01 ");
            TwAddVarRW(bar, "Color", TW_TYPE_COLOR3F, &colour1, " label='Particle color' ");
			TwAddVarRW(bar, "Finished?", TW_TYPE_BOOLCPP, &fin, " label='Finish with the effect' ");
			
		} 
		
		void dynnRenderer::makeInterface()
		{
			
			TwAddVarRW(thisBar, "Direction", TW_TYPE_DIR3F, &direction, "opened=true axisz=-z ");
			TwAddVarRW(thisBar, "Number Particles", TW_TYPE_INT32, &numbParticles ," label='Number of Particles' keyIncr=c keyDecr=C help='Defines the number of parts in the scene.' ");
			TwAddVarRW(thisBar, "Radius", TW_TYPE_FLOAT, &radiusRange,"min=0 max=180"); 
			TwAddVarRW(thisBar, "Source.x", TW_TYPE_FLOAT, &localPosition.x,"group='Position'  step=0.01 ");
			TwAddVarRW(thisBar, "Source.y", TW_TYPE_FLOAT, &localPosition.y,"group='Position'  step=0.01 ");
			TwAddVarRW(thisBar, "Source.z", TW_TYPE_FLOAT, &localPosition.z,"group='Position'  step=0.01 ");
            TwAddVarRW(thisBar, "Color", TW_TYPE_COLOR3F, &colour1, " label='Particle color' ");
			TwAddVarRW(thisBar, "Finished?", TW_TYPE_BOOLCPP, &fin, " label='Finish with the effect' ");
		}
		
		
		void dynnRenderer::removeInterface()
		{
			//TwRemoveAllVars(bar);
			TwRemoveVar(thisBar, "Direction") ;
			TwRemoveVar(thisBar, "Number Particles") ;
			TwRemoveVar(thisBar, "Radius") ;
			TwRemoveVar(thisBar, "Source.x") ;
			TwRemoveVar(thisBar, "Source.y") ;
			TwRemoveVar(thisBar, "Source.z") ;
			TwRemoveVar(thisBar, "Color") ;
			TwRemoveVar(thisBar, "Finished?") ;
			fin = false;
		}
		
  		  
  	}     
            
  }         