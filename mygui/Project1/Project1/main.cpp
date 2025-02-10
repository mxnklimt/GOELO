#include"Auto.h"

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
     ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simkai.ttf", 25.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    
// Our state


    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
           // ImGui::ShowDemoWindow(&show_demo_window);
            

       
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            //添加棋手
            static std::vector<ChessPlayer> players;
            static std::vector<ChessPlayer> players_notclear;
            static std::vector<ChessGame> games;
            static std::vector<ChessGame> mygames;
            // 固定窗口大小并禁用调整
            ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always); // 强制尺寸
            ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);   // 固定窗口位置（稍微偏离左上角）

            // 美化窗口样式
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);  // 设置圆角
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f); // 设置边框宽度
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.13f, 0.13f, 1.0f)); // 背景颜色
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // 边框颜色
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.36f, 0.68f, 0.87f, 1.0f)); // 按钮颜色
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.47f, 0.79f, 0.97f, 1.0f)); // 按钮悬停颜色
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.29f, 0.59f, 0.82f, 1.0f)); // 按钮点击颜色

            ImGui::Begin(u8"漏勺批发商");
            static bool dataLoaded = false;
            if (!dataLoaded) {
                FileManager::loadPlayerData(players, "players.txt");
                dataLoaded = true;  // 只加载一次
                players_notclear = players;
            }

            FileManager::loadGameData(mygames, "mygame.txt");

            // 文本和复选框
            //ImGui::ProgressBar(1, ImVec2(ImGui::GetFontSize() * 20, 0.0f));
			//第一个参数是值，第二个参数是进度条的大小
            
            //我要遍历players_notclear[i].name，找到name是“王恺愉”的players_notclear[i].elo
            float elonow=1500;
			std::string myname = "王恺愉";
            
            elonow = players_notclear[24].elo; //myelo

            ImGui::ProgressBar((int)elonow%200*0.005, ImVec2(ImGui::GetFontSize() * 20, 0.0f));
            ImGui::SameLine();
            static int duan = (elonow-1000)/200+2;

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // 红色
     
			ImGui::Text(u8"%d段",duan);
       
            // 恢复字体和颜色
            ImGui::PopStyleColor();

            ImGui::Spacing(); // 增加间距
            //ImGui::Checkbox("Demo Window", &show_demo_window);
            //ImGui::Checkbox("Another Window", &show_another_window);

            // 滑块和颜色选择
            ImGui::SliderFloat("ELO", &elonow, 600+duan*200, 800+duan*200); // 更改为更具描述性的标签
            //ImGui::ColorEdit3("Clear Color", (float*)&clear_color); // 更具描述性的标签

            // 绘制直方图
            
             for (size_t i = 0; i < games.size(); ++i) {
                std::cout <<"ELO Change: "
                    << games[i].elo_change << std::endl;
            }
             float datainit = 600 + duan * 200;
            float data[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
			//std::cout << "mygames.size()=" << mygames.size() << std::endl;
			//5局size=5
			for (int i = mygames.size(); i >=0; i--)
			{

                if (i == mygames.size() - 1)
                {
                    data[i] = elonow - mygames[i].elo_change;
                }
                if (i == mygames.size())
                {
                    data[i] = elonow;
                }
                else 
                {
                    data[i] = data[i + 1] - mygames[i].elo_change;
                }
				
			}

            int data_count = sizeof(data) / sizeof(data[0]);
            ImGui::PlotHistogram("ELO_History", data, data_count, 0, NULL, (600 + duan * 200), (800 + duan * 200), ImVec2(0, 80));
            mygames.clear();
            // 按钮和计数器
            //if (ImGui::Button("Increment Counter"))
            //    counter++;
            //ImGui::SameLine();
            //ImGui::Text("Counter = %d", counter);

            // 帧率显示
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            



            ImGui::End();
            
            // 恢复样式
            ImGui::PopStyleVar(2);  // 恢复之前的窗口样式
            ImGui::PopStyleColor(5); // 恢复之前的颜色样式

            //-------------------------------------------window2 begin-------------------------------------------
            // 创建右侧窗口模板
            ImGui::SetNextWindowPos(ImVec2(640, 20), ImGuiCond_Always);  // 设置右边窗口位置
            ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always); // 设置右边窗口尺寸

            ImGui::Begin("Right Window");

            // 在右边窗口中显示内容
            //ImGui::Text("This is a secondary window.");
            //ImGui::Spacing();

            // Load existing player data from file when starting
            
            
            PlayerInputWindow playerInputWindow;
            playerInputWindow.display(players);

			
            ////遍历打印players_notclear中的棋手姓名
            //for (int i = 0; i < players_notclear.size(); i++)
            //{
            //    std::cout << players_notclear[i].name << std::endl;
            //}

			static bool dataLoaded2 = false;
			if (!dataLoaded2)
			{
				FileManager::loadGameData(games, "games.txt");
				dataLoaded2 = true;
			}
			//遍历打印games中的比赛信息
            /*for (int i = 0; i < games.size(); i++)
            {
				std::cout << games[i].to_string() << std::endl;
            }
			std::cout << "--------------games end--------------" << std::endl;*/

            PlayerInputWindow playerInputWindow2;
            playerInputWindow2.display2(games,players_notclear);
            //games.clear();
            players.clear();
            ImGui::End();
            //-------------------------------------------window3 begin-------------------------------------------
            ImGui::SetNextWindowPos(ImVec2(20, 420), ImGuiCond_Always);  // 设置新窗口位置
            ImGui::SetNextWindowSize(ImVec2(400, 350), ImGuiCond_Always); // 设置新窗口尺寸

            ImGui::Begin("Chess Player Information"); // 新窗口的标题

            // 加载棋手数据
            static std::vector<ChessPlayer> players2;
            if (players2.empty())
            {
                FileManager::loadPlayerData(players2, "players.txt");
            }

            // 如果棋手库为空
            if (players2.empty()) {
                ImGui::Text("No players loaded.");
            }
            else {
                // 创建表格来显示棋手信息
                ImGui::Columns(3, "PlayerColumns"); // 创建3列的表格

                // 设置表头
                ImGui::Text("Name"); ImGui::NextColumn();
                ImGui::Text("Rating"); ImGui::NextColumn();
                ImGui::Text("ELO"); ImGui::NextColumn();

                ImGui::Separator(); // 列表分隔线

                int index = 0;
                // 遍历并显示所有棋手数据
                for (const auto& player : players2) {
                    if (index < 24)
                    {
                        index++;
                        continue;
                    }
                    bool isHovered = false;
                    bool isClicked = false;

                    // 检查鼠标是否悬停在当前棋手的名字上
                    ImGui::PushID(player.name.c_str()); // 推入一个独特的ID来避免冲突
                    isHovered = ImGui::IsItemHovered();  // 检查当前项是否被悬停
                    isClicked = ImGui::IsItemClicked(); // 检查当前项是否被点击
                    if (isHovered) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.0f, 1.0f)); // 设置高亮颜色
                    }

                    // 绘制棋手名字
                    ImGui::Text("%s", player.name.c_str());
                    ImGui::NextColumn();

                    // 还原颜色
                    if (isHovered) {
                        ImGui::PopStyleColor();
                    }

                    // 显示棋手其他信息
                    ImGui::Text("%d", player.rating); ImGui::NextColumn();
                    ImGui::Text("%.2f", player.elo); ImGui::NextColumn();

                    // 如果被点击，可以执行一些操作（例如选中棋手）
                    if (isClicked) {
                        // 这里可以处理点击事件，比如显示棋手的详细信息
                        // 例如：selectPlayer(player);
                    }

                    ImGui::PopID(); // 弹出ID
                }
                
                players2.clear();
                ImGui::Columns(1); // 恢复默认单列布局
            }
            
            ImGui::End(); // 结束窗口

            //-------------------------------------------window4 begin-------------------------------------------
            // 在控制台打印 games 里面是否有数据
            

            //for (size_t i = 0; i < games.size(); ++i) {
            //    std::cout << "Game " << i + 1 << ": "
            //        << games[i].player1.name << " vs "
            //        << games[i].player2.name << " - "
            //        << games[i].result << ", ELO Change: "
            //        << games[i].elo_change << std::endl;
            //}

            ImGui::SetNextWindowPos(ImVec2(430, 420), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(550, 350), ImGuiCond_Always);
            ImGui::Begin("Game List");

            // 调整表格样式，确保表头和数据行不会挤在一起
            if (ImGui::BeginTable("GameTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
                // **调整列宽分配**
                ImGui::TableSetupColumn("Player 1", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Player 2", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Result", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("ELO Change", ImGuiTableColumnFlags_WidthFixed, 140.0f);

                ImGui::TableHeadersRow();  // **确保表头行有间距，不会团在一起**

                // 遍历游戏数据
                for (const auto& game : games) {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, 25); // 增加行高，提高可读性

                    // 第一列：玩家1
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(game.player1.name.c_str());

                    // 第二列：玩家2
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(game.player2.name.c_str());

                    // 第三列：比赛结果（颜色区分胜负）
                    ImGui::TableSetColumnIndex(2);
                    if (game.result == "1") {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));  // 绿色 (胜)
                        ImGui::Text("Win!");
                    }
                    else if (game.result == "0") {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));  // 红色 (负)
                        ImGui::Text("Lose");
                    }
                    ImGui::PopStyleColor();

                    // 第四列：ELO 变化（右对齐）
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%.2f", game.elo_change);
                }

                ImGui::EndTable();
            }

            ImGui::End();





        }


        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
       


        // Rendering


        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
