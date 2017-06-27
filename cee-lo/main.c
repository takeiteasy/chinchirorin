//
//  main.c
//  cee-lo
//
//  Created by George Watson on 25/06/2017.
//  Copyright © 2017 George Watson. All rights reserved.
//

#include <stdio.h>

#include "glad.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <ode/ode.h>

#include "helpers.h"
#include "camera.h"
#include "obj.h"
#include "vector.h"

#include <png.h>

static const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

static SDL_Window* window;
static SDL_GLContext context;
static dWorldID world;
static dSpaceID space;
static dJointGroupID contact_group;
#define MAX_CONTACTS 8
static dContact contact[MAX_CONTACTS];
static vector_t dice;

#ifdef GLAD_DEBUG
void pre_gl_call(const char *name, void *funcptr, int len_args, ...) {
	printf("Calling: %s (%d arguments)\n", name, len_args);
}

char* glGetError_str(GLenum err) {
	switch (err) {
		case GL_INVALID_ENUM:                  return "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 return "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             return "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION"; break;
		default:
			return "Unknown Error";
	}
}

void post_gl_call(const char *name, void *funcptr, int len_args, ...) {
	GLenum err = glad_glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "ERROR %d (%s) in %s\n", err, glGetError_str(err), name);
		abort();
	}
}
#endif

void cleanup() {
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);
  dWorldDestroy(world);
  dSpaceDestroy(space);
  dJointGroupDestroy(contact_group);
	printf("Goodbye!\n");
}

void collide(void* data, dGeomID o1, dGeomID o2) {
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  
  if (b1 && b2 && dAreConnectedExcluding (b1, b2, dJointTypeContact))
    return;
  
  for (int i = 0; i < MAX_CONTACTS; ++i) {
    contact[i].surface.mode = dContactBounce;
    contact[i].surface.mu = dInfinity;
    contact[i].surface.mu2 = 0;
    contact[i].surface.bounce = 0.2;
    contact[i].surface.bounce_vel = 0.1;
  }

  int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
  if (numc) {
    for (int i = 0; i < numc; i++) {
      dJointID c = dJointCreateContact(world, contact_group, contact + i);
      dJointAttach (c, b1, b2);
    }
  }
}

typedef struct {
  dGeomID geom;
  dBodyID body;
  mat4 world;
  obj_t* model;
  GLuint texture;
} game_obj_t;

void draw_game_obj(game_obj_t* o, GLuint model_loc, GLuint texture_loc) {
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, &o->world.m[0]);
  
  if (texture_loc && o->texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, o->texture);
    glUniform1i(texture_loc, 0);
  }
  draw_obj(o->model);
  
  glBindTexture(GL_TEXTURE_2D, 0);
}

void free_game_obj(game_obj_t* o) {
  dGeomDestroy(o->geom);
  if (o->model) {
    free_obj(o->model);
    o->model = NULL;
  }
  if (o->texture)
    glDeleteTextures(1, &o->texture);
}

int main(int argc, const char * argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to initalize SDL!\n");
		return -1;
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	window = SDL_CreateWindow("im not gay",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if (!window) {
		fprintf(stderr, "Failed to create SDL window!\n");
		return -1;
	}
	
	context = SDL_GL_CreateContext(window);
	if (!context) {
		fprintf(stderr, "Failed to create OpenGL context!\n");
		return -1;
	}
	
	if (!gladLoadGL()) {
		fprintf(stderr, "Failed to load GLAD!\n");
		return -1;
	}
	
#ifdef GLAD_DEBUG
//	glad_set_pre_callback(pre_gl_call);
//	glad_set_post_callback(post_gl_call);
#endif
	
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
	printf("GLSL:     %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	glClearColor(220.0f/255.0f, 220.0f/255.0f, 220.0f/255.0f, 1.0f);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	atexit(cleanup);
	
	mat4 p = mat4_perspective(45.f, .1f, 1000.f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
  camera_t cam;
  camera_init_def(&cam);
  cam.pos.z = 5.f;
  cam.pos.y = 1.f;
	
	GLuint shader = load_shader_str(
GLSL(330,
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
                                       
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main() {
  gl_Position = projection * view * model* vec4(aPos, 1.0);
  TexCoord = vec2(aTexCoord.x, -aTexCoord.y);
}),
GLSL(330,
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main() {
  FragColor = texture(ourTexture, TexCoord);
}));
  
  int cube_tex_w, cube_tex_h;
	GLuint cube_tex  = load_texture("/Users/rusty/git/cee-lo/res/dice.tga", &cube_tex_w, &cube_tex_h);
  
	obj_t cube_obj, plane_obj;
	load_obj(&cube_obj,  "/Users/rusty/git/cee-lo/res/dice.obj");
  load_obj(&plane_obj, "/Users/rusty/git/cee-lo/res/plane.obj");
  
  vector_init(&dice);
  
	GLuint projection_loc = glGetUniformLocation(shader, "projection");
	GLuint view_loc = glGetUniformLocation(shader, "view");
	GLuint model_loc = glGetUniformLocation(shader, "model");
  GLuint texture_loc = glGetUniformLocation(shader, "ourTexture");
	
  dInitODE();
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	dWorldSetGravity(world, 0.f, -9.81f, 0.f);
  dWorldSetLinearDamping(world, 0.0001);
  dWorldSetAngularDamping(world, 0.005);
  dWorldSetMaxAngularSpeed(world, 200);
  dWorldSetContactMaxCorrectingVel(world, 0.1);
  dWorldSetContactSurfaceLayer(world, 0.001);
  contact_group = dJointGroupCreate(0);
  
  game_obj_t plane;
  plane.geom = dCreatePlane(space, 0, 1, 0, 0);
  plane.model = &plane_obj;
  plane.texture = 0;
  plane.world = mat4_id();
	
	Uint32 old_time, current_time = SDL_GetTicks();
	float delta;
	SDL_bool running = SDL_TRUE, running_physics;
	const Uint8* keys;
	SDL_Event e;
	while (running) {
		old_time = current_time;
		current_time = SDL_GetTicks();
		delta = (float)(current_time - old_time) / 1000.0f;
		
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					running = SDL_FALSE;
					break;
        case SDL_MOUSEMOTION:
          camera_look(&cam, e.motion.xrel, -e.motion.yrel);
          break;
        case SDL_KEYUP:
          if (e.key.keysym.sym == SDLK_z) {
            game_obj_t* tmp = (game_obj_t*)malloc(sizeof(game_obj_t));
            tmp->body = dBodyCreate(world);
            dMass mass;
            dMassSetBox(&mass, 1.0f, 1.0f, 1.0f, 1.0f);
            dBodySetMass(tmp->body, &mass);
            dBodySetPosition(tmp->body, 0.0f, 30.f, 0.0f);
            dMatrix3 R;
            dRFromAxisAndAngle(R, 0.2, 0.4, 0.6, M_PI / 4.0);
            dBodySetRotation(tmp->body, R);
            
            tmp->geom = dCreateBox(space, 0.5f, 0.5f, 0.5f);
            dGeomSetBody(tmp->geom, tmp->body);
            
            tmp->model = &cube_obj;
            tmp->texture = cube_tex;
            
            vector_push(&dice, (void*)tmp);
          }
          break;
			}
		}
		
		keys = SDL_GetKeyboardState(NULL);
		
		if (keys[SDL_GetScancodeFromKey(SDLK_b)])
			printf("BREAK\n");
		
    running_physics = SDL_TRUE;
		if (keys[SDL_GetScancodeFromKey(SDLK_ESCAPE)])
			running = SDL_FALSE;
		
    if (keys[SDL_GetScancodeFromKey(SDLK_SPACE)])
      running_physics = SDL_FALSE;
    
    if (keys[SDL_GetScancodeFromKey(SDLK_w)])
      camera_move(&cam, FORWARD);
    if (keys[SDL_GetScancodeFromKey(SDLK_a)])
      camera_move(&cam, LEFT);
    if (keys[SDL_GetScancodeFromKey(SDLK_s)])
      camera_move(&cam, BACK);
    if (keys[SDL_GetScancodeFromKey(SDLK_d)])
      camera_move(&cam, RIGHT);
    if (keys[SDL_GetScancodeFromKey(SDLK_q)])
      camera_move(&cam, UP);
    if (keys[SDL_GetScancodeFromKey(SDLK_e)])
      camera_move(&cam, DOWN);
    
    if (running_physics) {
      dSpaceCollide(space, 0, collide);
      dWorldStep(world, 1.f / 60.f);
      dJointGroupEmpty (contact_group);
    }
		
    camera_update(&cam);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(shader);
    
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &p.m[0]);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &cam.view.m[0]);
    
    for (int i = 0; i < dice.length; ++i) {
      game_obj_t* tmp = (game_obj_t*)vector_get(&dice, i);
      
      const dReal* t = dBodyGetPosition(tmp->body);
      const dReal* r = dBodyGetRotation(tmp->body);
      
      tmp->world = mat4_mul_mat4(mat4_new(r[0], r[4], r[8],  t[0],
                                          r[1], r[5], r[9],  t[1],
                                          r[2], r[6], r[10], t[2],
                                          0.f,  0.f,  0.f,   1.f),
                                 mat4_scale(vec3_new(0.25f, 0.25f, 0.25f)));
      
      draw_game_obj(tmp, model_loc, texture_loc);
    }

    draw_game_obj(&plane, model_loc, 0);
    
    glUseProgram(0);
		
		SDL_GL_SwapWindow(window);
	}
  
  for (int i = 0; i < dice.length; ++i)
    free_game_obj((game_obj_t*)vector_get(&dice, i));
  vector_free(&dice);
  free_game_obj(&plane);
  glDeleteProgram(shader);
  
	return 0;
}
