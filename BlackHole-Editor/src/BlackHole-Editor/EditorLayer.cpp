#include "BlackHole-Editor/EditorLayer.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/type_ptr.inl"


EditorLayer::EditorLayer()
    : Layer("EditorLayer")
    , m_CameraController(PerspectiveCamera(45.0f,
        static_cast<float>(Application::Get().GetWindow().GetWidth()) / static_cast<float>(Application::Get().GetWindow().GetHeight()),
        0.1f, 100.0f))
{
}

void EditorLayer::OnAttach()
{
	FramebufferSpecification fbSpec;
	fbSpec.Width = Application::Get().GetWindow().GetWidth();
	fbSpec.Height = Application::Get().GetWindow().GetHeight();
	fbSpec.Attachments = {
		{
		    FramebufferTextureFormat::RGBA8,
			{
				{ FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
			    { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
				{ FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
                { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
			}
		},
		{
			FramebufferTextureFormat::DEPTH24,
		    {
		        { FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
		        { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
		        { FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
		        { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
		    }
        
		}
	};
	m_FinalResultFBO = CreateRef<Framebuffer>(fbSpec);

	fbSpec.Attachments = {
		{
		    FramebufferTextureFormat::RGBA16F,
			{
				{ FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
			    { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
				{ FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
                { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
			}
		},
		{
		    FramebufferTextureFormat::RGBA16F,
			{
				{ FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
			    { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
				{ FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
                { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
			}
		},
		{
		    FramebufferTextureFormat::RGBA16F,
			{
				{ FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
			    { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
				{ FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
                { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
			}
		},
		{
			FramebufferTextureFormat::DEPTH24,
		    {
		        { FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
		        { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
		        { FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
		        { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
		    }
        
		}
	};
	m_GBufferFBO = CreateRef<Framebuffer>(fbSpec);

	fbSpec.Attachments = {
		{
		    FramebufferTextureFormat::RGBA16F,
			{
				{ FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
			    { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
				{ FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
                { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
			}
		},
	    {
		    FramebufferTextureFormat::RGBA16F,
			{
				{ FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
			    { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
				{ FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
                { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
			}
		},
		{
			FramebufferTextureFormat::DEPTH24,
		    {
		        { FramebufferTextureParameterName::FILTER_MAG, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
		        { FramebufferTextureParameterName::FILTER_MIN, FramebufferTextureParameterValue::FILTER_TYPE_NEAREST },
		        { FramebufferTextureParameterName::WRAP_S, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE },
		        { FramebufferTextureParameterName::WRAP_T, FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE }
		    }
        
		}
	};
	m_PingPongPostProcessFBO = CreateRef<Framebuffer>(fbSpec);

	m_FloorModel = CreateRef<Model>(Filesystem::GetModelsPath() / "scene/floor/floor.obj");
	m_CarModel = CreateRef<Model>(Filesystem::GetModelsPath() / "scene/covered_car_1k/covered_car_1k.obj");
	m_BarrelModel = CreateRef<Model>(Filesystem::GetModelsPath() / "scene/barrel_stove_1k/barrel_stove_1k.obj");
	m_UtilityBoxModel = CreateRef<Model>(Filesystem::GetModelsPath() / "scene/utility_box_02_1k/utility_box_02_1k.obj");
	m_PointLightModel = CreateRef<Model>(Filesystem::GetModelsPath() / "scene/point_light/sphere.obj");

	ShaderSpecification shSpec;
	shSpec.VertexPath = Filesystem::GetShadersPath() / "Deferred shading/geometryPass.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "Deferred shading/geometryPass.fs.glsl";
	Renderer::GetShaderLibrary().Load("GeometryPass", shSpec);
	Renderer::GetShaderLibrary().Get("GeometryPass")->UploadInt("u_Material.Diffuse", 0);
	Renderer::GetShaderLibrary().Get("GeometryPass")->UploadInt("u_Material.Normal", 1);
	Renderer::GetShaderLibrary().Get("GeometryPass")->UploadInt("u_Material.Displacement", 2);

	shSpec.VertexPath = Filesystem::GetShadersPath() / "Deferred shading/lightingPass.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "Deferred shading/lightingPass.fs.glsl";
	Renderer::GetShaderLibrary().Load("LightingPass", shSpec);
	Renderer::GetShaderLibrary().Get("LightingPass")->UploadInt("u_Position", 0);
	Renderer::GetShaderLibrary().Get("LightingPass")->UploadInt("u_Normal", 1);
	Renderer::GetShaderLibrary().Get("LightingPass")->UploadInt("u_AlbedoSpec", 2);

	Renderer::GetShaderLibrary().Load(Filesystem::GetShadersPath() / "PointLightSource.glsl");

	shSpec.VertexPath = Filesystem::GetShadersPath() / "screenSquad.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "Deferred shading/lightingMap.fs.glsl";
	Renderer::GetShaderLibrary().Load("LightingMap", shSpec);
	Renderer::GetShaderLibrary().Get("LightingMap")->UploadInt("u_Position", 0);
	Renderer::GetShaderLibrary().Get("LightingMap")->UploadInt("u_Normal", 1);
	Renderer::GetShaderLibrary().Get("LightingMap")->UploadInt("u_AlbedoSpec", 2);
	Renderer::GetShaderLibrary().Get("LightingMap")->UploadInt("u_LightMap", 3);
	Renderer::GetShaderLibrary().Get("LightingMap")->UploadInt("u_Skybox", 4);

	shSpec.VertexPath = Filesystem::GetShadersPath() / "screenSquad.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "HDRtoLDR.fs.glsl";
	Renderer::GetShaderLibrary().Load("HDRtoLDR", shSpec);
	Renderer::GetShaderLibrary().Get("HDRtoLDR")->UploadInt("u_Scene", 0);

	shSpec.VertexPath = Filesystem::GetShadersPath() / "screenSquad.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "gaussianBlur.fs.glsl";
	Renderer::GetShaderLibrary().Load("GaussianBlur", shSpec);
	Renderer::GetShaderLibrary().Get("GaussianBlur")->UploadInt("u_Scene", 0);

	CubemapSpecification cbSpec;
    cbSpec.Right  = Filesystem::GetTexturesPath() / "skyboxes/mountains/right.jpg";
    cbSpec.Left   = Filesystem::GetTexturesPath() / "skyboxes/mountains/left.jpg";
    cbSpec.Top    = Filesystem::GetTexturesPath() / "skyboxes/mountains/top.jpg";
    cbSpec.Bottom = Filesystem::GetTexturesPath() / "skyboxes/mountains/bottom.jpg";
    cbSpec.Front  = Filesystem::GetTexturesPath() / "skyboxes/mountains/front.jpg";
    cbSpec.Back   = Filesystem::GetTexturesPath() / "skyboxes/mountains/back.jpg";
	m_SkyboxCubemap = CreateRef<Cubemap>(cbSpec);

	PointLight pointLight;

	pointLight.Position = { -1.0f, 1.5f, -1.0f };
	pointLight.Color = { 0.0f, 1.0f, 0.0f };
	pointLight.Linear = 0.22f;
	pointLight.Quadratic = 0.2f;
	pointLight.Intensity = 1.0f;
	m_PointLightsInfo.push_back(pointLight);

	pointLight.Position = { 1.0f, 1.0f, -2.0f };
	pointLight.Color = { 1.0f, 0.0f, 0.0f };
	pointLight.Linear = 0.7f;
	pointLight.Quadratic = 1.8f;
	pointLight.Intensity = 1.0f;
	m_PointLightsInfo.push_back(pointLight);

	
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(Timestep ts)
{
	m_FrameTime = ts;

    if (m_ViewportFocused)
		m_CameraController.OnUpdate(ts);

	const auto& shaderLib = Renderer::GetShaderLibrary();

	// Resize
    const FramebufferSpecification& spec = m_FinalResultFBO->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
		&& (spec.Width != static_cast<uint32_t>(m_ViewportSize.x) || spec.Height != static_cast<uint32_t>(m_ViewportSize.y)))
	{
		m_FinalResultFBO->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_GBufferFBO->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_PingPongPostProcessFBO->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_CameraController.OnResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}

	Renderer::ResetStats();

	// Geometry pass
	{
	    m_GBufferFBO->Bind();
		Renderer::Clear();
		m_GBufferFBO->ClearColorAttachment(0, static_cast<float*>(nullptr));
		m_GBufferFBO->ClearColorAttachment(1, static_cast<float*>(nullptr));

		// Drawing all meshes to obtain info for G-buffer
		Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, -0.5f, 0.0));
        transform = glm::scale(transform, glm::vec3(10.0f, 1.0f, 10.0f));
        Renderer::Submit(m_FloorModel, shaderLib.Get("GeometryPass"), transform);

		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(3.0f, 0.0f, 0.0f));
		Renderer::Submit(m_CarModel, shaderLib.Get("GeometryPass"), transform);

		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(-2.0f, 0.0f, -1.5f));
		Renderer::Submit(m_BarrelModel, shaderLib.Get("GeometryPass"), transform);

		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(-3.0f, 0.0f, 2.0f));
		transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		Renderer::Submit(m_UtilityBoxModel, shaderLib.Get("GeometryPass"), transform);

		Renderer::EndScene();

		m_GBufferFBO->Unbind();
	}

	// Lighting pass
	m_PingPongPostProcessFBO->Bind();
	{
		// Generating lighting map
		Renderer::Clear();
		m_PingPongPostProcessFBO->ClearColorAttachment<float>(0, nullptr);
		m_PingPongPostProcessFBO->ClearColorAttachment<float>(1, nullptr);

		Renderer::BeginScene(m_CameraController.GetCamera());

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
        glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
        glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);

        glBindTextureUnit(0, m_GBufferFBO->GetColorAttachmentRendererID());
		glBindTextureUnit(1, m_GBufferFBO->GetColorAttachmentRendererID(1));
		glBindTextureUnit(2, m_GBufferFBO->GetColorAttachmentRendererID(2));
        for (const auto& pointLight : m_PointLightsInfo)
        {
			const float lightMax = glm::max(glm::max(pointLight.Color.r, pointLight.Color.g), pointLight.Color.b);
			const float lightVolumeRadius = 
				(-pointLight.Linear +  glm::sqrt(pointLight.Linear * pointLight.Linear - 4.0f * pointLight.Quadratic * (1.0f - (256.0f / 5.0f) * lightMax))) / (2.0f * pointLight.Quadratic);
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, pointLight.Position);
            transform = glm::scale(transform, glm::vec3(lightVolumeRadius));

            shaderLib.Get("LightingPass")->UploadFloat3("u_PointLight.Position", pointLight.Position);
            shaderLib.Get("LightingPass")->UploadFloat3("u_PointLight.Color", pointLight.Color * pointLight.Intensity);
			shaderLib.Get("LightingPass")->UploadFloat("u_PointLight.Linear", pointLight.Linear);
			shaderLib.Get("LightingPass")->UploadFloat("u_PointLight.Quadratic", pointLight.Quadratic);
			shaderLib.Get("LightingPass")->UploadFloat("u_PointLight.VolumeRadius", lightVolumeRadius);

			Renderer::Submit(m_PointLightModel, shaderLib.Get("LightingPass"), transform);
        }
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		Renderer::EndScene();

		// Blurring the lighting map
		glDisable(GL_DEPTH_TEST);
		Renderer::BeginScene(m_CameraController.GetCamera());
		shaderLib.Get("GaussianBlur")->UploadFloat("u_ViewportWidth", static_cast<float>(spec.Width));
		shaderLib.Get("GaussianBlur")->UploadFloat("u_ViewportHeight", static_cast<float>(spec.Height));
		Renderer::RenderScreenQuad(shaderLib.Get("GaussianBlur"), {
			m_PingPongPostProcessFBO->GetColorAttachmentRendererID()
		});
		Renderer::EndScene();

	    // Applying lighting map
		Renderer::BeginScene(m_CameraController.GetCamera());
		m_SkyboxCubemap->Bind(4);
		Renderer::RenderScreenQuad(shaderLib.Get("LightingMap"), {
			m_GBufferFBO->GetColorAttachmentRendererID(),
			m_GBufferFBO->GetColorAttachmentRendererID(1),
			m_GBufferFBO->GetColorAttachmentRendererID(2),
			m_PingPongPostProcessFBO->GetColorAttachmentRendererID()
		});
		Renderer::EndScene();
		glEnable(GL_DEPTH_TEST);
	}

	// Forward lighting pass
    {
		Renderer::BeginScene(m_CameraController.GetCamera());
		m_PingPongPostProcessFBO->BlitFramebufferDepthAttachment(m_GBufferFBO);

		for (const auto& pointLight : m_PointLightsInfo)
        {
			glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, pointLight.Position);
            transform = glm::scale(transform, glm::vec3(0.1f));
		    shaderLib.Get("PointLightSource")->UploadFloat3("u_LightSourceColor", pointLight.Color * pointLight.Intensity);
		    Renderer::Submit(m_PointLightModel, shaderLib.Get("PointLightSource"), transform);
        }
		
		Renderer::EndScene();
    }
	m_PingPongPostProcessFBO->Unbind();

	// Converting from HDR to LDR
	{
	    m_FinalResultFBO->Bind();
		Renderer::Clear();

		glDisable(GL_DEPTH_TEST);
		Renderer::RenderScreenQuad(shaderLib.Get("HDRtoLDR"),{
			m_PingPongPostProcessFBO->GetColorAttachmentRendererID(1)
		});
		glEnable(GL_DEPTH_TEST);

		m_FinalResultFBO->Unbind();
	}
}

void EditorLayer::OnImGuiRender()
{
	static bool dockspaceOpen = true;
	static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	static constexpr ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Dockspace Demo", &dockspaceOpen, windowFlags);
	ImGui::PopStyleVar(3);

	// DockSpace
    const ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
    const float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
        const ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				Application::Get().Close();
			
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
    }

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
	ImGui::PopStyleVar();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	const bool viewportIsReadyForInteraction = m_ViewportFocused || m_ViewportHovered;
    if (viewportIsReadyForInteraction && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		ImGui::SetWindowFocus();

    Application::Get().GetImGuiLayer()->BlockEvents(!viewportIsReadyForInteraction);

    const uint64_t textureID = m_FinalResultFBO->GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2( m_ViewportSize.x, m_ViewportSize.y ), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::End();

    ImGui::Begin("Stats");
    const auto stats = Renderer::GetStats();
    ImGui::Text("Frame time: %f s", m_FrameTime);
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Triangles: %d", stats.TriangleCount);
	ImGui::Text("Lines: %d", stats.LinesCount);
	ImGui::Text("Points: %d", stats.PointsCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

	ImGui::Begin("Light props[0]");
	ImGui::DragFloat3("Position", glm::value_ptr(m_PointLightsInfo[0].Position), 0.01f);
	ImGui::ColorEdit3("Color", glm::value_ptr(m_PointLightsInfo[0].Color));
	ImGui::DragFloat("Linear", &m_PointLightsInfo[0].Linear, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Quadratic", &m_PointLightsInfo[0].Quadratic, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Intensity", &m_PointLightsInfo[0].Intensity, 0.01f, 0.0f, 10.0f);
	ImGui::End();

	ImGui::Begin("Light props[1]");
	ImGui::DragFloat3("Position", glm::value_ptr(m_PointLightsInfo[1].Position), 0.01f);
	ImGui::ColorEdit3("Color", glm::value_ptr(m_PointLightsInfo[1].Color));
	ImGui::DragFloat("Linear", &m_PointLightsInfo[1].Linear, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Quadratic", &m_PointLightsInfo[1].Quadratic, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Intensity", &m_PointLightsInfo[1].Intensity, 0.01f, 0.0f, 10.0f);
	ImGui::End();

	ImGui::End();
}

void EditorLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}