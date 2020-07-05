#include "SoftRender\graphic.h"
#include "SoftRender\setting.h"
//Screen dimension constants


void SDLDrawPixel(int x, int y)
{
    SDL_RenderDrawPoint(gRenderer, x, SCREEN_HEIGHT - 1 - y);
}



void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
    V2F left, right, top;
    left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
    right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
    top = v3;
    left.windowPos.x = int(left.windowPos.x);
    int dy = top.windowPos.y - left.windowPos.y;
    int nowY = top.windowPos.y;
    //从上往下插值
    for (int i = dy; i >= 0; i--) {
        float weight = 0;
        if (dy != 0) {
            weight = (float)i / dy;
        }
        V2F newLeft = V2F::lerp(left, top, weight);
        V2F newRight = V2F::lerp(right, top, weight);
        newLeft.windowPos.x = int(newLeft.windowPos.x);
        newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
        newLeft.windowPos.y = newRight.windowPos.y = nowY;
        ScanLine(newLeft, newRight);
        nowY--;
    }
}

void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
    V2F left, right, bottom;
    left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
    right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
    bottom = v3;
    int dy = left.windowPos.y - bottom.windowPos.y;
    int nowY = left.windowPos.y;
    //从上往下插值
    for (int i = 0; i < dy; i++) {
        float weight = 0;
        if (dy != 0) {
            weight = (float)i / dy;
        }
        V2F newLeft = V2F::lerp(left, bottom, weight);
        V2F newRight = V2F::lerp(right, bottom, weight);
        newLeft.windowPos.x = int(newLeft.windowPos.x);
        newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
        newLeft.windowPos.y = newRight.windowPos.y = nowY;
        ScanLine(newLeft, newRight);
        nowY--;
    }
}

void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
    std::vector<V2F> arr = { v1,v2,v3 };
    if (arr[0].windowPos.y > arr[1].windowPos.y) {
        V2F tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
    if (arr[1].windowPos.y > arr[2].windowPos.y) {
        V2F tmp = arr[1];
        arr[1] = arr[2];
        arr[2] = tmp;
    }
    if (arr[0].windowPos.y > arr[1].windowPos.y) {
        V2F tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
    //arr[0] 在最下面  arr[2]在最上面
    //中间跟上面的相等，是底三角形
    if (arr[1].windowPos.y == arr[2].windowPos.y) {
        DownTriangle(arr[1], arr[2], arr[0]);
    }//顶三角形
    else if (arr[1].windowPos.y == arr[0].windowPos.y) {
        UpTriangle(arr[1], arr[0], arr[2]);
    }
    else {
        //插值求出中间点对面的那个点，划分为两个新的三角形
        float weight = (arr[2].windowPos.y - arr[1].windowPos.y) / (arr[2].windowPos.y - arr[0].windowPos.y);
        V2F newEdge = V2F::lerp(arr[2], arr[0], weight);
        UpTriangle(arr[1], newEdge, arr[2]);
        DownTriangle(arr[1], newEdge, arr[0]);
    }
}

void ScanLine(const V2F& left, const V2F& right) {
    int length = right.windowPos.x - left.windowPos.x;
    for (int i = 0; i < length; i++) {
        V2F v = V2F::lerp(left, right, (float)i / length);
        v.windowPos.x = left.windowPos.x + i;
        v.windowPos.y = left.windowPos.y;

        SDL_SetRenderDrawColor(gRenderer, shader.FragmentShader(v).x, shader.FragmentShader(v).y, shader.FragmentShader(v).z, shader.FragmentShader(v).w);
        SDLDrawPixel(v.windowPos.x, v.windowPos.y);
    }
}


bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SoftRender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

void close()
{
    //Destroy window    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;
			glm::mat4 ViewPortMatrix = GetViewPortMatrix(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			FrontBuffer.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);


			Vertex V1(glm::vec3(-0.5, -0.5, 0), glm::vec4(255, 0, 0, 0), glm::vec2(0,0), glm::vec3(0, 0,0));
			Vertex V2(glm::vec3(0.5, 0.5, 0), glm::vec4(255, 255, 255, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0));
			Vertex V3(glm::vec3(-0.5, 0.5, 0), glm::vec4(0,0, 255, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0));

			V2F o1 = shader.VertexShader(V1);
			V2F o2 = shader.VertexShader(V2);
			V2F o3 = shader.VertexShader(V3);

			o1.windowPos = ViewPortMatrix * o1.windowPos;
			o2.windowPos = ViewPortMatrix * o2.windowPos;
			o3.windowPos = ViewPortMatrix * o3.windowPos;

            //While application is running
            while (!quit)
            {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
                SDL_RenderClear(gRenderer);

				
                //Rendering
                ScanLineTriangle(o1, o2, o3);
               

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}