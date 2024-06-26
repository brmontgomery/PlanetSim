#include "PSIMAppLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PSIMAppLayer::PSIMAppLayer()
	: Layers("PSIMAppLayer"), m_CameraController(1280.0f / 720.0f)
{
	m_AssetLibs = PSIMAssetLibraries::getAssetLibraries();

	m_VertexArray = VertexArray::Create();

	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float3, "a_Color" }
	};

	std::cout << layout.GetStride() << std::endl;
	m_AssetLibs->PSIM_ModelLibrary.Load("C:\\dev\\PlanetSim\\assets\\models\\chalet.obj", false, true, false, false);

	//default vertexArray creation
	{
		Ref<VertexBuffer> vertexBuffer;
		{
			float* vertices = static_cast<float*>(m_AssetLibs->PSIM_ModelLibrary.Get("chalet")->Data.data());
			uint32_t modelSize = m_AssetLibs->PSIM_ModelLibrary.Get("chalet")->Data.size();
			vertexBuffer = VertexBuffer::Create(vertices, modelSize);
		}
		vertexBuffer->SetLayout(layout);

		Ref<IndexBuffer> indexBuffer;
		{
			uint32_t* indices = m_AssetLibs->PSIM_ModelLibrary.Get("chalet")->indices.data();
			uint32_t indexSize = m_AssetLibs->PSIM_ModelLibrary.Get("chalet")->indices.size();
			indexBuffer = IndexBuffer::Create(indices, indexSize);
		}

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_AssetLibs->PSIM_TextureLibrary.Load("C:\\dev\\PlanetSim\\assets\\textures\\chalet.jpg");
		m_AssetLibs->PSIM_TextureLibrary.bindTexture("chalet");
	}

	/*std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	m_Shader = Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	m_FlatColorShader = Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
	*/
}

void PSIMAppLayer::OnAttach()
{
}

void PSIMAppLayer::OnDetach()
{
}

void PSIMAppLayer::OnUpdate(Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	RenderCommands::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommands::SetViewMatrix(m_CameraController.GetCamera().getViewMatrix());
	RenderCommands::SetProjectionMatrix(m_CameraController.GetCamera().getProjectionMatrix());
	RenderCommands::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	/*
	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}

	//auto textureShader = m_ShaderLibrary.Get("Texture");

	//m_Texture->Bind();
	//Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	//m_ChernoLogoTexture->Bind();
	//Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	*/
	// Triangle
	Renderer::Submit(m_VertexArray);

	Renderer::EndScene();
}

void PSIMAppLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void PSIMAppLayer::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}