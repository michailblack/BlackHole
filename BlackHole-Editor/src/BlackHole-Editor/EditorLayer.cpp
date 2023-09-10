#include "BlackHole-Editor/EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
			{ FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST, FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST },
			{ FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE, FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE }
		},
		{
		    FramebufferTextureFormat::DEPTH24STENCIL8,
			{ FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST, FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST },
			{ FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE, FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE }
		}
	};
	m_ViewportFBO = CreateRef<Framebuffer>(fbSpec);

	fbSpec.Attachments = {
		{
		    FramebufferTextureFormat::RGBA16F,
			{ FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST, FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST },
			{ FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE, FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE }
		},
		{
		    FramebufferTextureFormat::RGBA16F,
			{ FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST, FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST },
			{ FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE, FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE }
		},
		{
		    FramebufferTextureFormat::RGBA16F,
			{ FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST, FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST },
			{ FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE, FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE }
		},
		{
		    FramebufferTextureFormat::DEPTH24,
			{ FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST, FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST },
			{ FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE, FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE }
		}
	};
	m_GBufferFBO = CreateRef<Framebuffer>(fbSpec);

	m_Model = CreateRef<Model>(Filesystem::GetModelsPath() / "primitives/cube/cube.obj");

	ShaderSpecification shSpec;
	shSpec.VertexPath = Filesystem::GetShadersPath() / "Deferred shading/geometryPass.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "Deferred shading/geometryPass.fs.glsl";
	Renderer::GetShaderLibrary().Load("Geometry", shSpec);

	shSpec.VertexPath = Filesystem::GetShadersPath() / "screenSquad.vs.glsl";
	shSpec.FragmentPath = Filesystem::GetShadersPath() / "Deferred shading/lightningPass.fs.glsl";
	Renderer::GetShaderLibrary().Load("Lightning", shSpec);
	Renderer::GetShaderLibrary().Get("Lightning")->UploadInt("u_Position", 0);
	Renderer::GetShaderLibrary().Get("Lightning")->UploadInt("u_Normal", 1);
	Renderer::GetShaderLibrary().Get("Lightning")->UploadInt("u_AlbedoSpec", 2);

	m_PointLights = CreateRef<UniformBuffer>(3 * 4 * sizeof(float), 1);
	auto* const mapPointLights = static_cast<glm::vec4*>(m_PointLights->Map(0, 2 * sizeof(float)));
	*mapPointLights = glm::vec4(-1.0f, 0.0f, -1.0f, 0.0f);
	*(mapPointLights + 1) = glm::vec4(0.0f, 100.0f, 100.0f, 0.0f);
	*(mapPointLights + 2) = glm::vec4(0.7f, 1.8f, 0.0f, 0.0f);
	m_PointLights->Unmap();
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
    const FramebufferSpecification& spec = m_ViewportFBO->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
		&& (spec.Width != static_cast<uint32_t>(m_ViewportSize.x) || spec.Height != static_cast<uint32_t>(m_ViewportSize.y)))
	{
		m_ViewportFBO->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_GBufferFBO->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_CameraController.OnResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}

	Renderer::ResetStats();

	// Geometry path
	{
	    m_GBufferFBO->Bind();
		Renderer::Clear();

		Renderer::BeginScene(m_CameraController.GetCamera());
		shaderLib.Get("Geometry")->Bind();
		shaderLib.Get("Geometry")->UploadMat4("u_Model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
		Renderer::Submit(m_Model);
		Renderer::EndScene();

		m_GBufferFBO->Unbind();
	}

	// Lightning path
    {
        m_ViewportFBO->Bind();
	    Renderer::Clear();

		shaderLib.Get("Lightning")->Bind();
		shaderLib.Get("Lightning")->UploadFloat3("u_CameraPos", m_CameraController.GetCamera().GetPosition());
		Renderer::RenderScreenQuad({ m_GBufferFBO->GetColorAttachmentRendererID(), m_GBufferFBO->GetColorAttachmentRendererID(1), m_GBufferFBO->GetColorAttachmentRendererID(2) });

	    m_ViewportFBO->Unbind();
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

    const uint64_t textureID = m_ViewportFBO->GetColorAttachmentRendererID();
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

	/*const auto& shaderLibrary = Renderer::GetShaderLibrary();
	ImGui::Begin("Light Properties");
	ImGui::DragFloat3("Direction", glm::value_ptr(m_LightDirection), 0.01f);
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(m_LightDiffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(m_LightSpecular));
	ImGui::DragFloat2("Width", glm::value_ptr(m_OrthoWidth));
	ImGui::DragFloat2("Height", glm::value_ptr(m_OrthoHeight));
	ImGui::DragFloat2("Depth", glm::value_ptr(m_OrthoDepth));
	ImGui::End();

	shaderLibrary.Get("Model")->UploadFloat3("u_LightDirection", m_LightDirection);
    shaderLibrary.Get("Model")->UploadFloat3("u_DirectionalLight.Diffuse"  , m_LightDiffuse);
    shaderLibrary.Get("Model")->UploadFloat3("u_DirectionalLight.Specular" , m_LightSpecular);*/

	ImGui::End();
}

void EditorLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}