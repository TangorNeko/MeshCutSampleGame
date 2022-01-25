#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"
#include "ExEngine.h"

#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

#include "SoundEngine.h"
#include "ModelCutManager.h"
#include "SkinModelRender.h"
#include "SoundSource.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "LightManager.h"
#include "ResourceBankManager.h"
#include "NameKey.h"
#include "RenderingEngine.h"

const Vector3 GAME_GRAVITY = { 0.0f,-500.0f,0.0f };