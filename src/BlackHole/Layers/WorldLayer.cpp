#include "WorldLayer.h"

WorldLayer::WorldLayer()
    : Layer("WorldLayer")
    , m_CameraController(PerspectiveCamera(45.0f,
        static_cast<float>(Application::Get().GetWindow().GetWidth()) / static_cast<float>(Application::Get().GetWindow().GetHeight()),
        0.1f, 100.0f))
{
}


size_t amount = 1'000;
glm::mat4 *modelMatrices;

void WorldLayer::OnAttach()
{
    {
        m_PointsVAO = CreateRef<VertexArray>();
        float vertices[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
        };

        uint32_t indices[] = {
            0, 1, 2, 3
        };

        m_PointsVBO = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
        m_PointsVBO->SetLayout({
            { ShaderDataType::Float2, "a_Position" },
            { ShaderDataType::Float3, "a_Color"    }
        });

        m_PointsIBO = CreateRef<IndexBuffer>(indices, static_cast<uint32_t>(sizeof(indices) / sizeof(uint32_t)));

        m_PointsVAO->AddVertexBuffer(m_PointsVBO);
        m_PointsVAO->SetIndexBuffer(m_PointsIBO);
    }

    {
        m_ScreenSquadVAO = CreateRef<VertexArray>();
        float vertices[] = {
            -1.0f, -1.0f,    0.0f, 0.0f,
             1.0f, -1.0f,    1.0f, 0.0f,
             1.0f,  1.0f,    1.0f, 1.0f,
            -1.0f,  1.0f,    0.0f, 1.0f,
        };

        uint32_t indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        m_ScreenSquadVBO = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
        m_ScreenSquadVBO->SetLayout({
            { ShaderDataType::Float2, "a_Position"      },
            { ShaderDataType::Float2, "a_TextureCoords" }
        });

        m_ScreenSquadIBO = CreateRef<IndexBuffer>(indices, static_cast<uint32_t>(sizeof(indices) / sizeof(uint32_t)));

        m_ScreenSquadVAO->AddVertexBuffer(m_ScreenSquadVBO);
        m_ScreenSquadVAO->SetIndexBuffer(m_ScreenSquadIBO);
    }

    {
        m_CubeVAO = CreateRef<VertexArray>();
        float vertices[] = {
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        uint32_t indices[] = {
#if 0
            0, 1, 2,
            2, 3, 0,

            1, 5, 6,
            6, 2, 1,

            5, 4, 7,
            7, 6, 5,

            4, 0, 3,
            3, 7, 4,

            3, 2, 6,
            6, 7, 3,

            4, 5, 1,
            1, 0, 4
#else
            4, 5, 6,
            6, 7, 4,

            5, 1, 2,
            2, 6, 5,

            0, 4, 7,
            7, 3, 0,

            1, 0, 3,
            3, 2, 1,

            7, 6, 2,
            2, 3, 7,

            0, 1, 5,
            5, 4, 0
#endif
        };

        m_CubeVBO = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
        m_CubeVBO->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
        });

        m_CubeIBO = CreateRef<IndexBuffer>(indices, static_cast<uint32_t>(sizeof(indices) / sizeof(uint32_t)));

        m_CubeVAO->AddVertexBuffer(m_CubeVBO);
        m_CubeVAO->SetIndexBuffer(m_CubeIBO);
    }
    auto& shaderLibrary = Renderer::GetShaderLibrary();

    shaderLibrary.Load("../../../assets/shaders/screenSquad.glsl");
    shaderLibrary.Load("../../../assets/shaders/skybox.glsl");
    shaderLibrary.Load("../../../assets/shaders/solidColor.glsl");
    shaderLibrary.Load("../../../assets/shaders/modelNormals.glsl");
    const auto& modelShader = shaderLibrary.Load("../../../assets/shaders/model.glsl");

    m_Model = CreateRef<Model>("../../../assets/models/planet/planet.obj");
    m_Rock = CreateRef<Model>("../../../assets/models/rock/rock.obj");

    modelShader->Bind();
    modelShader->UploadFloat3("dirLight.direction", glm::vec3(1.0f, -1.0f, 1.0f));
    modelShader->UploadFloat3("dirLight.ambient"  , glm::vec3(0.2f));
    modelShader->UploadFloat3("dirLight.diffuse"  , glm::vec3(0.8f));
    modelShader->UploadFloat3("dirLight.specular" , glm::vec3(0.5f));

    modelShader->UploadFloat("u_Material.shininess", 0.2f);
    Shader::Unbind();

    modelMatrices = new glm::mat4[amount];
    srand(time(0)); // initialize random seed	
    float radius = 50.0;
    float offset = 2.5f;
    for(unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }  
}

void WorldLayer::OnDetach()
{
}

void WorldLayer::OnUpdate(Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    const auto& shaderLibrary = Renderer::GetShaderLibrary();

    Renderer::BeginScene(m_CameraController.GetCamera());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

    Renderer::Submit(shaderLibrary.Get("model"), m_Model, model);
    for (size_t i = 0; i < amount; ++i)
        Renderer::Submit(shaderLibrary.Get("model"), m_Rock, modelMatrices[i]);
    Renderer::Submit(shaderLibrary.Get("skybox"), m_CubeVAO, glm::mat4(1.0f));
    Renderer::EndScene();
}

void WorldLayer::OnImGuiRender()
{
}

void WorldLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}