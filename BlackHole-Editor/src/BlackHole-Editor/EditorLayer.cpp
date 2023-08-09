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
    m_Model = CreateRef<Model>(Filesystem::GetModelsPath() / "BarberShopChair_01_8k/BarberShopChair_01_8k.fbx");

    FramebufferSpecification fbSpec;
    fbSpec.Width = Application::Get().GetWindow().GetWidth();
    fbSpec.Height = Application::Get().GetWindow().GetHeight();
    fbSpec.Samples = 4;
    m_FramebufferMSAA = CreateRef<Framebuffer>(fbSpec);

	fbSpec.Samples = 1;
	m_Framebuffer = CreateRef<Framebuffer>(fbSpec);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(Timestep ts)
{
    m_FPS = 1.0f / ts;

    if (m_ViewportFocused)
		m_CameraController.OnUpdate(ts);

    // Resize
    const FramebufferSpecification& spec = m_FramebufferMSAA->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
		&& (spec.Width != static_cast<uint32_t>(m_ViewportSize.x) || spec.Height != static_cast<uint32_t>(m_ViewportSize.y)))
	{
		m_FramebufferMSAA->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_Framebuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_CameraController.OnResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}

    glm::mat4 model = glm::scale(glm::mat4(1.0f), m_ModelScale);
	model = glm::rotate(model, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, m_ModelTranslation);

    m_FramebufferMSAA->Bind();
    m_FramebufferMSAA->ClearColorAttachment({ 0.2f, 0.2f, 0.2f, 1.0f });
    m_FramebufferMSAA->ClearDepthAttachment();

	Renderer::ResetStats();

    Renderer::BeginScene(m_CameraController.GetCamera());
    Renderer::Submit(m_Model, model);
    Renderer::DrawSkybox();
    Renderer::EndScene();

    m_FramebufferMSAA->Unbind();

	m_Framebuffer->BlitFramebuffer(m_FramebufferMSAA);
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

	const uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2( m_ViewportSize.x, m_ViewportSize.y ), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::End();

	ImGui::Begin("Stats");
    const auto stats = Renderer::GetStats();
    ImGui::Text("FPS: %f", m_FPS);
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Triangles: %d", stats.TriangleCount);
	ImGui::Text("Lines: %d", stats.LinesCount);
	ImGui::Text("Points: %d", stats.PointsCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

	ImGui::Begin("Properties");
	ImGui::DragFloat3("Translation", glm::value_ptr(m_ModelTranslation), 0.1f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(m_ModelRotation), 1.0f, -180.0f, 180.0f);
	ImGui::DragFloat3("Scale", glm::value_ptr(m_ModelScale), 0.01f, 0.1f, 10.0f);
	ImGui::End();

	ImGui::End();
}

void EditorLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}