#ifndef OVERDRIVE_APP_RENDERTEST_H
#define OVERDRIVE_APP_RENDERTEST_H

#include "app/application.h"
#include "render/shaderprogram.h"
#include "render/vertexarray.h"
#include "render/vertexbuffer.h"
#include "input/keyboard.h"
#include "opengl.h"

namespace overdrive {
	namespace app {
		class RenderTest :
			public Application,
			public core::MessageHandler<input::Keyboard::OnKeyPress>
		{
		public:
			RenderTest();

			bool initialize();
			void update();
			void shutdown();

			void operator()(const input::Keyboard::OnKeyPress& kp);

		private:
			void createCube(float size = 1.0f);

			glm::mat4 mModel;
			glm::mat4 mView;
			glm::mat4 mProjection;

			float mAngle;

			render::ShaderProgram mShaderProgram;

			// openGL resources cannot be created until after engine initialization is completed!
			std::unique_ptr<render::VertexBuffer> mVertices;
			std::unique_ptr<render::VertexBuffer> mNormals;
			std::unique_ptr<render::VertexBuffer> mTexCoords;
			std::unique_ptr<render::VertexBuffer> mIndices;
			GLsizei mNumElements;

			std::unique_ptr<render::VertexArray> mVertexArray;
		};
	}
}

#endif
