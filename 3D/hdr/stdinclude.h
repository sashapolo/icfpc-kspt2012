#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>

#include <irrlicht.h>

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

using namespace std;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "Logger.h"
#include "IrrLogger.h"

#include "Point.h"
#include "Path.h"
#include "FieldMember.h"
#include "Field.h"

#include "FieldSim.h"

#include "Utilities.h"
#include "MeshBuffer.h"

#include "ShaderBumpCallback.h"
#include "ShaderParallaxCallback.h"
#include "PostBlurCallback.h"
#include "PostBloomCallback.h"

#include "LifterScene.h"
#include "LifterGUI.h"
