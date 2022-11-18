#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>


#include "OpenglUtils.h"

#define TILE_SIZE = 8
#define PI 3.14159265359
#define HALF_PI PI/2
#define PI3 3*PI/2 //idk what to call that
#define TAU PI*2

#define ONE_DEGREE 0.0174533

unsigned int SQUARE_INDICES[] = 
{
	0,1,3,
	1,2,3
};

int gameMap[] = 
{ 
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 0, 1, 1,
	1, 0, 0, 1, 0, 0, 1, 1,
	1, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 0, 1, 0, 0, 1,
	1, 0, 1, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,

};

void simplePrint(std::string s)
{
	std::cout << s << std::endl;
}

//This is gross but it is a simple solution so ignore
//,playerKey,playerLoSKey,
enum VAO_TYPE {floorVAO=0,wallVAO=1,playerVAO=2,horizontalLineVAO,verticalLineVAO,scene3DWallVAO, scene3DShadedWallVAO, VAO_MAX_SIZE };
enum ATTRIBUTES {position=0,color=1};

struct Player {
	float x, y;
	float angle;
	float delta_x, delta_y;

};


Player player{ 64+8.0f,64+8.0f,0,cos(player.angle),player.delta_y = sin(player.angle) };

void initGraphics(unsigned int vao[],unsigned int vbo[],unsigned int ebo[])
{

	//Positions, and colors for both floorTile and wallTile
	float floorTile[] = {
		0.0f,	64.0f,	0.0f,		1.0f, 1.0f, 1.0f, // top right
		64.0f,	64.0f,	0.0f,		1.0f, 1.0f, 1.0f, // bottom right
		64.0f,	0.0f,	0.0f,		1.0f, 1.0f, 1.0f, // bottom left
		0.0f,	0.0f,	0.0f,		1.0f, 1.0f, 1.0f, // top left
	};

	float wallTile[] = {
		0.0f,	64.0f,	0.0f,		0.0f, 0.0f, 0.0f, // top right
		64.0f,	64.0f,	0.0f,		0.0f, 0.0f, 0.0f, // bottom right
		64.0f,	0.0f,	0.0f,		0.0f, 0.0f, 0.0f, // bottom left
		0.0f,	0.0f,	0.0f,		0.0f, 0.0f, 0.0f, // top left
	};

	float player[] = {
		0.0f,	8.0f,	0.0f,		1.0f, 0.6f, 0.0f, // top right
		8.0f,	8.0f,	0.0f,		1.0f, 0.6f, 0.0f, // bottom right
		8.0f,	0.0f,	0.0f,		0.3f, 0.6f, 0.1f, // bottom left
		0.0f,	0.0f,	0.0f,		0.3f, 0.6f, 0.1f, // top left
	};

	float horizontalLOS[] = {
		0.0f,	0.0f,	0.0f,		.62f, 0.12f, 1.0f, // top right
		8.0f,	0.0f,	0.0f,		.62f, 0.12f, 1.0f, // bottom right
	};

	float verticalLOS[] = {
	0.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f, // top right
	8.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f, // bottom right
	};

	float sceneWall[] = {
		0.0f,	8.0f,	0.0f, 1.0f, 0.0f, 0.0f,// top right
		8.0f,	8.0f,	0.0f, 1.0f, 0.0f, 0.0f,// bottom right
		8.0f,	0.0f,	0.0f, 1.0f, 0.0f, 0.0f,// bottom left
		0.0f,	0.0f,	0.0f, 1.0f, 0.0f, 0.0f,// top left
	};

	float sceneShadedWall[] = {
		0.0f,	8.0f,	0.0f, 0.7f, 0.0f, 0.0f,// top right
		8.0f,	8.0f,	0.0f, 0.7f, 0.0f, 0.0f,// bottom right
		8.0f,	0.0f,	0.0f, 0.7f, 0.0f, 0.0f,// bottom left
		0.0f,	0.0f,	0.0f, 0.7f, 0.0f, 0.0f,// top left
	};

	glGenVertexArrays(VAO_MAX_SIZE, vao);
	glGenBuffers(VAO_MAX_SIZE, vbo);
	glGenBuffers(VAO_MAX_SIZE, ebo);

	//Gen Floor
	glBindVertexArray(vao[floorVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[floorVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorTile), floorTile, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SQUARE_INDICES), SQUARE_INDICES, GL_STATIC_DRAW);

	glVertexAttribPointer(position,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(position);	
	
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(color);	
	
	//Gen Wall
	glBindVertexArray(vao[wallVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[wallVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wallTile), wallTile, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[wallVAO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SQUARE_INDICES), SQUARE_INDICES, GL_STATIC_DRAW);

	glVertexAttribPointer(position,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(position);	
	
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(color);

	//Gen Player
	glBindVertexArray(vao[playerVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[playerVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[playerVAO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SQUARE_INDICES), SQUARE_INDICES, GL_STATIC_DRAW);

	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(position);

	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(color);

	//Gen player line of sight horizontal

	glBindVertexArray(vao[horizontalLineVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[horizontalLineVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(horizontalLOS), horizontalLOS, GL_STATIC_DRAW);

	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(position);

	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(color);

	//Gen player line of sight verical

	glBindVertexArray(vao[verticalLineVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[verticalLineVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticalLOS), verticalLOS, GL_STATIC_DRAW);

	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(position);

	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(color);

	// Create the 3D wall VAO
	glBindVertexArray(vao[scene3DWallVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[scene3DWallVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sceneWall), sceneWall, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[scene3DWallVAO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SQUARE_INDICES), SQUARE_INDICES, GL_STATIC_DRAW);

	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(position);

	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(color);


	// Create the Shaded 3D wall VAO
	glBindVertexArray(vao[scene3DShadedWallVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[scene3DShadedWallVAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sceneShadedWall), sceneShadedWall, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[scene3DShadedWallVAO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SQUARE_INDICES), SQUARE_INDICES, GL_STATIC_DRAW);

	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(position);

	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(color);

}

float distance(float x1, float y1, float x2, float y2, float angle)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void drawPlayer(Shader shader, unsigned int playerVAO)
{
	//Model - modify obj
	glm::mat4 model = glm::mat4(1.0f);

	//View - modify camera
	glm::mat4 view = glm::mat4(1.0f);

	//Projection - change projection type						 
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 512.0f, 0.0f);//glm::mat4(1.0f); 

	//MVP matrix
	glm::mat4 mvp = glm::mat4(1.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(player.x,player.y, .0f));
	mvp = projection * view * model;
	shader.bind();
	glBindVertexArray(playerVAO);
	shader.setMatrix4f("mvp", 1, GL_FALSE, value_ptr(mvp));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//shader.unbind();

}

void draw_block(Shader shader, unsigned int sceneVAO, int loc, float scaleFactor)
{
	glm::mat4 model_3d = glm::mat4(1.0f);
	model_3d = glm::translate(model_3d, glm::vec3(513 + (loc * 8), 50-scaleFactor/2, 0));
	model_3d = glm::scale(model_3d, glm::vec3(1, scaleFactor, 0));
	//View - modify camera
	glm::mat4 view_3d = glm::mat4(1.0f);
	//Projection - change projection type						 
	glm::mat4 projection_3d = glm::ortho(1.0f, 1024.0f, 512.0f, 0.0f);//glm::mat4(1.0f); 
	glm::mat4 mvp_3d = glm::mat4(1.0f);

	mvp_3d = projection_3d * view_3d * model_3d;

	glm::vec3 wallColor = glm::vec3(1.0f, 0.0f, 0.0f);

	shader.bind();
	glBindVertexArray(sceneVAO);
	//shader.setVec3f("userColor", 1, value_ptr(wallColor));
	shader.setMatrix4f("mvp", 1, GL_FALSE, value_ptr(mvp_3d));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void drawRays(Shader shader, Shader colorShader,unsigned int hVAO, unsigned int vVAO, float distances[], bool isSide[])
{
	//Model - modify obj
	glm::mat4 model = glm::mat4(1.0f);
	//View - modify camera
	glm::mat4 view = glm::mat4(1.0f);
	//Projection - change projection type						 
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 512.0f, 0.0f);//glm::mat4(1.0f); 
	glm::mat4 mvp = glm::mat4(1.0f);

	glm::mat4 pModel = glm::mat4(1.0f);

	
	pModel = glm::translate(pModel, glm::vec3(player.x + 4, player.y + 4, 0));
	pModel = glm::rotate(pModel, player.angle, glm::vec3(0, 0, 1));
	
	mvp = projection * view * pModel;
	glBindVertexArray(hVAO);
	shader.bind();
	shader.setMatrix4f("mvp", 1, GL_FALSE, value_ptr(mvp));
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	//shader.unbind();

	glBindVertexArray(vVAO);

	int rayCount = 10;

	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo;
	
	ra = player.angle - ONE_DEGREE*30;

	if (ra < 0)
	{
		ra += TAU;
	}

	if (ra > TAU)
	{
		ra -= TAU;
	}

	for (int i = 0; i <= 64; i++)
	{
		//std::cout << i  <<std::endl;
		// --- Horizontal lines ---
		dof = 0;
		float aTan = -1 / tan(ra);
		float disH = 1000000, hx = player.x, hy = player.y;

		if (ra > PI) // looking down on the grid
		{
			ry = (((int)player.y >> 6) << 6) - 0.0001;
			rx = (player.y - ry) * aTan + player.x;

			yo = -64;
			xo = -yo * aTan;
		}

		if (ra < PI)
		{
			ry = (((int)player.y >> 6) << 6)+64;
			rx = (player.y - ry) * aTan + player.x;

			yo = 64;
			xo = -yo * aTan;
		}

		if (ra == 0 || ra == PI)
		{
			rx = player.x;
			ry = player.y;
		}

		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;

			//gameMap[8 * c + r] gives the position
			mp = my * 8 + mx;

			if (mp > 0 && mp < 8 * 8 && gameMap[mp] == 1)
			{
				dof = 8;

				hx = rx;
				hy = ry;
				disH = distance(player.x, player.y, hx, hy, ra);
			}
			else
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}


		/// --- Vertical lines ---
		dof = 0;
		float nTan = -tan(ra);
		float disV = 1000000, vx = player.x, vy = player.y;
		if (ra > HALF_PI && ra < PI3) // looking left 
		{
			rx = (((int)player.x >> 6) << 6) - 0.0001;
			ry = (player.x - rx) * nTan + player.y;

			xo = -64;
			yo = -xo * nTan;
		}

		else if (ra < PI || ra > PI3) // looking right 
		{
			rx = (((int)player.x >> 6) << 6) + 64;
			ry = (player.x - rx) * nTan + player.y;

			xo = 64;
			yo = -xo * nTan;
		}

		if (ra == 0 || ra == PI) // up or down (straight)
		{
			rx = player.x;
			ry = player.y;
			dof = 8;
		}

		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;

			mp = my * 8 + mx;

			if (mp > 0 && mp < 8 * 8 && gameMap[mp] == 1)
			{
				dof = 8;

				vx = rx;
				vy = ry;
				disV = distance(player.x, player.y, vx, vy, ra);
			}
			else
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}

		ra += ONE_DEGREE;

		if (ra < 0)
		{
			ra += TAU;
		}

		if (ra > TAU)
		{
			ra -= TAU;
		}

		/// Render the 2d scene rays
		float shortest_distance;
		if (disV < disH)
		{
			rx = vx;
			ry = vy;
			shortest_distance = disV;
			isSide[i] = false;
		}
		else
		{
			rx = hx;
			ry = hy;
			shortest_distance = disH;
			isSide[i] = true;
		}


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(player.x + 4, player.y + 4, 0));
		model = glm::scale(model, glm::vec3(shortest_distance / 8, shortest_distance / 8, 0));
		//model = glm::rotate(model, player.angle, glm::vec3(0, 0, 1));
		model = glm::rotate(model, ra, glm::vec3(0, 0, 1));

		mvp = projection * view * model;

		shader.bind();
		shader.setMatrix4f("mvp", 1, GL_FALSE, value_ptr(mvp));
		glDrawArrays(GL_LINE_STRIP, 0, 2);
		//shader.unbind();


		/// --- Draw 3D Scene ---



		//shader.setMatrix4f("mvp", 1, GL_FALSE, value_ptr(mvp_3d));
		//glBindVertexArray(sceneVAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//colorShader.unbind();


		//float lineH = (64 * 320) / shortest_distance;
		//if (lineH > 320)
		//{
		//	lineH = 320;
		//}
		float ca = player.angle - ra;
		if (ca < 0)
		{
			ca += TAU;
		}
		if (ca > TAU)
		{
			ca -= TAU;
		}
		shortest_distance = shortest_distance * cos(ca);
		distances[i] = shortest_distance;
		//draw_block(shader,sceneVAO, i);




	}

	//std::cout << distances << std::endl;

	//return distances;

}

void drawGameWorld(Shader shader, unsigned int floorVAO, unsigned int wallVAO)
{
	//Model - modify obj
	glm::mat4 model = glm::mat4(1.0f);
	
	//View - modify camera
	glm::mat4 view = glm::mat4(1.0f);

	//Projection - change projection type						 
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 512.0f, 0.0f); //glm::mat4(1.0f); 
	
	//MVP matrix
	glm::mat4 mvp = glm::mat4(1.0f);

	//Shader must be bound before uniforms
	shader.bind();

	//std::cout << " ====THE START =====" << std::endl;

	for (int c = 0; c < 8; c++)
	{
		for (int r = 0; r < 8; r++)
		{
			//std::cout << gameMap[8 * c + r];

			if (gameMap[8 * c + r] == 1)
			{
				glBindVertexArray(wallVAO);
			}
			else
			{
				glBindVertexArray(floorVAO);
			}

			model = glm::translate(glm::mat4(1.0f), glm::vec3(r * 64.0f, 64.0f * c, .0f));
			mvp = projection * view * model;
			shader.setMatrix4f("mvp", 1, GL_FALSE, value_ptr(mvp));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			//std::cout << "(" << r * 64 << "," << c * 64 << "), ";

		}

		//std::cout << std::endl;
	}

	//shader.unbind();

	//std::cout << " ====THE END =====" << std::endl;
}

void handleKeyPresses(GLFWwindow* window)
{
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window,true);
	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)) //up 
	{
		player.x += player.delta_x;
		player.y += player.delta_y;
		//player.y -= 5;
	}	
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)) //down
	{
		player.x -= player.delta_x;
		player.y -= player.delta_y;
		//player.y += 5;
	}	
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)) //right
	{
		player.angle += .05;
		if (player.angle > TAU)
		{
			player.angle -= TAU;
		}
		player.delta_x = cos(player.angle)*5;
		player.delta_y = sin(player.angle)*5;
		//player.x += 5;
	}	
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)) //left
	{	
		player.angle -= .05;
		if (player.angle < 0)
		{
			player.angle += TAU;
		}
		player.delta_x = cos(player.angle)*5;
		player.delta_y = sin(player.angle)*5;
		//player.x -= 5;
	}
}

int main()
{
	GLFWwindow* window = setupGLFW("Raycaster", "v1", 1024,512);

	Shader basicShader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	Shader colorShader("res/shaders/color/vColor.shader", "res/shaders/color/fColor.shader");
	
	unsigned int vao[VAO_MAX_SIZE], vbo[VAO_MAX_SIZE], ebo[VAO_MAX_SIZE];
	initGraphics(vao,vbo,ebo);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float distances[65];
	bool isSide[65];
	while (!glfwWindowShouldClose(window))
	{
		handleKeyPresses(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		drawGameWorld(basicShader, vao[floorVAO], vao[wallVAO]);
		drawPlayer(basicShader,vao[playerVAO]);
		drawRays(basicShader, colorShader, vao[horizontalLineVAO], vao[verticalLineVAO], distances, isSide);
		
		for (int i = 0; i <= 64; i++)
		{
			
			float lineScaleFactor =  1500 / distances[i];
			//isSide[i] ? vao[scene3DWallVAO] : vao[scene3DShadedWallVAO];
			draw_block(basicShader, isSide[i] ? vao[scene3DWallVAO] : vao[scene3DShadedWallVAO], i, lineScaleFactor);
		}
			
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}






