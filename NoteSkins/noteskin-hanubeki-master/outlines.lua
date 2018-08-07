--[[
	この作品はクリエイティブ・コモンズ・表示 - 継承 4.0 国際・ライセンスで
	提供されています。このライセンスのコピーを閲覧するには、
	http://creativecommons.org/licenses/by-sa/4.0/を訪問して下さい。

	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
	International License. To view a copy of this license,
	visit http://creativecommons.org/licenses/by-sa/4.0/.
]]

local skin_name = Var("skin_name");
return function(button_list, stepstype, skin_parameters)
	local ret = {};

	-- Image and rotation/flip table for buttons and note type
	local buttonInfoTable = {
		-- dance, pump, techno
		["Up"] = {
			baseWidth   = 64,
			tap         = {image = "down", rotZ = 180, rotY = 0},
			hold        = {image = "down", flip = "TexCoordFlipMode_Y"},
			holdReverse = {image = "down", flip = "TexCoordFlipMode_None"},
			roll        = {image = "up", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "down", flip = "TexCoordFlipMode_None"},
		},
		["Down"] = {
			baseWidth   = 64,
			tap         = {image = "down", rotZ = 0, rotY = 0},
			hold        = {image = "down", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "down", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "down", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "up", flip = "TexCoordFlipMode_None"},
		},
		["Left"] = {
			baseWidth   = 64,
			tap         = {image = "down", rotZ = 90, rotY = 0},
			hold        = {image = "left", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "left", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "left", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "left", flip = "TexCoordFlipMode_None"},
		},
		["Right"] = {
			baseWidth   = 64,
			tap         = {image = "down", rotZ = 270, rotY = 0},
			hold        = {image = "left", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "left", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "left", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "left", flip = "TexCoordFlipMode_X"},
		},
		["UpLeft"] = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 0, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "upleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "downleft", flip = "TexCoordFlipMode_None"},
		},
		["UpRight"] = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 90, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "upleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "downleft", flip = "TexCoordFlipMode_X"},
		},
		["DownLeft"]  = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 270, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_Y"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_None"},
			roll        = {image = "downleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "upleft", flip = "TexCoordFlipMode_None"},
		},
		["DownRight"] = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 180, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_XY"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_X"},
			roll        = {image = "downleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "upleft", flip = "TexCoordFlipMode_X"},
		},
		["Center"] = {
			baseWidth   = 64,
			tap         = {image = "center", rotZ = 0, rotY = 0},
			hold        = {image = "center", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "center", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "center", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "center", flip = "TexCoordFlipMode_None"},
		},
		-- para
		["ParaUp"] = {
			baseWidth   = 64,
			tap         = {image = "paraup", rotZ = 0, rotY = 0},
			hold        = {image = "paraup", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "paraup", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "paraup", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "paradown", flip = "TexCoordFlipMode_None"},
		},
		["ParaLeft"] = {
			baseWidth   = 64,
			tap         = {image = "paraup", rotZ = 270, rotY = 0},
			hold        = {image = "paraleft", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "paraleft", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "paraleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "paraleft", flip = "TexCoordFlipMode_None"},
		},
		["ParaRight"] = {
			baseWidth   = 64,
			tap         = {image = "paraup", rotZ = 90, rotY = 0},
			hold        = {image = "paraleft", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "paraleft", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "paraleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "paraleft", flip = "TexCoordFlipMode_X"},
		},
		["ParaUpLeft"]  = {
			baseWidth   = 64,
			tap         = {image = "paraupleft", rotZ = 0, rotY = 0},
			hold        = {image = "paraupleft", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "paraupleft", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "paraupleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "paradownleft", flip = "TexCoordFlipMode_None"},
		},
		["ParaUpRight"] = {
			baseWidth   = 64,
			tap         = {image = "paraupleft", rotZ = 90, rotY = 0},
			hold        = {image = "paraupleft", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "paraupleft", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "paraupleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "paradownleft", flip = "TexCoordFlipMode_X"},
		},
		-- ez2, ds3ddx
		["FootDown"] = {
			baseWidth   = 64,
			tap         = {image = "down", rotZ = 0, rotY = 0},
			hold        = {image = "down", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "down", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "down", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "up", flip = "TexCoordFlipMode_None"},
		},
		["FootUpLeft"] = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 0, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "upleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "downleft", flip = "TexCoordFlipMode_None"},
		},
		["FootUpRight"] = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 90, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "upleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "downleft", flip = "TexCoordFlipMode_X"},
		},
		["FootDownLeft"]  = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 270, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_Y"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_None"},
			roll        = {image = "downleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "upleft", flip = "TexCoordFlipMode_None"},
		},
		["FootDownRight"] = {
			baseWidth   = 64,
			tap         = {image = "upleft", rotZ = 180, rotY = 0},
			hold        = {image = "upleft", flip = "TexCoordFlipMode_XY"},
			holdReverse = {image = "upleft", flip = "TexCoordFlipMode_X"},
			roll        = {image = "downleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "upleft", flip = "TexCoordFlipMode_X"},
		},
		["HandUp"] = {
			baseWidth   = 64,
			tap         = {image = "handdown", rotZ = 180, rotY = 0},
			hold        = {image = "handdown", flip = "TexCoordFlipMode_Y"},
			holdReverse = {image = "handdown", flip = "TexCoordFlipMode_None"},
			roll        = {image = "handup", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "handdown", flip = "TexCoordFlipMode_None"},
		},
		["HandDown"] = {
			baseWidth   = 64,
			tap         = {image = "handdown", rotZ = 0, rotY = 0},
			hold        = {image = "handdown", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "handdown", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "handdown", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "handup", flip = "TexCoordFlipMode_None"},
		},
		["HandLeft"] = {
			baseWidth   = 64,
			tap         = {image = "handleft", rotZ = 0, rotY = 0},
			hold        = {image = "handleft", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "handleft", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "handleft", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "handleft", flip = "TexCoordFlipMode_None"},
		},
		["HandRight"] = {
			baseWidth   = 64,
			tap         = {image = "handleft", rotZ = 180, rotY = 0},
			hold        = {image = "handleft", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "handleft", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "handleft", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "handleft", flip = "TexCoordFlipMode_X"},
		},
		["HandUpLeft"] = {
			baseWidth   = 64,
			tap         = {image = "circle", rotZ = 0, rotY = 0},
			hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
		},
		["HandUpRight"] = {
			baseWidth   = 64,
			tap         = {image = "circle", rotZ = 0, rotY = 0},
			hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
		},
		["HandLrLeft"] = {
			baseWidth   = 64,
			tap         = {image = "circle", rotZ = 0, rotY = 0},
			hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
		},
		["HandLrRight"] = {
			baseWidth   = 64,
			tap         = {image = "circle", rotZ = 0, rotY = 0},
			hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
		},
		-- kb7, beat
		["Key1"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["Key2"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["Key3"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["Key4"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["Key5"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["Key6"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["Key7"] = {
			baseWidth   = 64,
			tap         = {image = "key", rotZ = 0, rotY = 0},
			hold        = {image = "key", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "key", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
		},
		["scratch"] = {
			baseWidth   = 128,
			tap         = {image = "scratch", rotZ = 0, rotY = 0},
			hold        = {image = "scratch", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "scratch", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "scratch", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "scratch", flip = "TexCoordFlipMode_None"},
		},
		-- popn
		["Left White"] = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Left Yellow"]  = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Left Green"] = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Left Blue"] = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Red"] = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Right Blue"] = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Right Green"]  = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Right Yellow"] = {
			baseWidth   = 64,
			tap         = {image = "burger", rotZ = 0, rotY = 0},
			hold        = {image = "burger", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		["Right White"]  = {
			baseWidth   = 64,
			tap         = {image = "burger", flip = "TexCoordFlipMode_None"},
			hold        = {image = "burger", rotZ = 0, rotY = 0},
			holdReverse = {image = "burger", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "burger", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "burger", flip = "TexCoordFlipMode_None"},
		},
		-- kickbox
		["DownLeftFoot"]  = { -- Down: blue
			baseWidth   = 64,
			tap         = {image = "rightfoot", rotZ = 0, rotY = 180}, -- Down: blue
			hold        = {image = "rightfoot", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "rightfoot", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "rightfoot", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "reverserightfoot", flip = "TexCoordFlipMode_X"},
		},
		["LeftFoot"] = { -- Any:  yellow
			baseWidth   = 64,
			tap         = {image = "rightfoot", rotZ = 0, rotY = 180}, -- Any:  yellow
			hold        = {image = "rightfoot", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "rightfoot", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "rightfoot", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "reverserightfoot", flip = "TexCoordFlipMode_X"},
		},
		["UpLeftFoot"] = { -- Up:   red
			baseWidth   = 64,
			tap         = {image = "rightfoot", rotZ = 0, rotY = 180}, -- Up:   red
			hold        = {image = "rightfoot", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "rightfoot", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "rightfoot", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "reverserightfoot", flip = "TexCoordFlipMode_X"},
		},
		["UpLeftFist"] = {
			baseWidth   = 64,
			tap         = {image = "rightfist", rotZ = 0, rotY = 180},
			hold        = {image = "rightfist", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "rightfist", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "rightfist", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "reverserightfist", flip = "TexCoordFlipMode_X"},
		},
		["LeftFist"] = {
			baseWidth   = 64,
			tap         = {image = "rightfist", rotZ = 0, rotY = 180},
			hold        = {image = "rightfist", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "rightfist", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "rightfist", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "reverserightfist", flip = "TexCoordFlipMode_X"},
		},
		["DownLeftFist"]  = {
			baseWidth   = 64,
			tap         = {image = "rightfist", rotZ = 0, rotY = 180},
			hold        = {image = "rightfist", flip = "TexCoordFlipMode_X"},
			holdReverse = {image = "rightfist", flip = "TexCoordFlipMode_XY"},
			roll        = {image = "rightfist", flip = "TexCoordFlipMode_X"},
			rollReverse = {image = "reverserightfist", flip = "TexCoordFlipMode_X"},
		},
		["DownRightFist"] = {
			baseWidth   = 64,
			tap         = {image = "rightfist", rotZ = 0, rotY = 0},
			hold        = {image = "rightfist", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "rightfist", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "rightfist", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "reverserightfist", flip = "TexCoordFlipMode_None"},
		},
		["RightFist"] = {
			baseWidth   = 64,
			tap         = {image = "rightfist", rotZ = 0, rotY = 0},
			hold        = {image = "rightfist", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "rightfist", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "rightfist", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "reverserightfist", flip = "TexCoordFlipMode_None"},
		},
		["UpRightFist"] = {
			baseWidth   = 64,
			tap         = {image = "rightfist", rotZ = 0, rotY = 0},
			hold        = {image = "rightfist", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "rightfist", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "rightfist", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "reverserightfist", flip = "TexCoordFlipMode_None"},
		},
		["UpRightFoot"] = {
			baseWidth   = 64,
			tap         = {image = "rightfoot", rotZ = 0, rotY = 0},
			hold        = {image = "rightfoot", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "rightfoot", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "rightfoot", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "reverserightfoot", flip = "TexCoordFlipMode_None"},
		},
		["RightFoot"] = {
			baseWidth   = 64,
			tap         = {image = "rightfoot", rotZ = 0, rotY = 0},
			hold        = {image = "rightfoot", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "rightfoot", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "rightfoot", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "reverserightfoot", flip = "TexCoordFlipMode_None"},
		},
		["DownRightFoot"] = {
			baseWidth   = 64,
			tap         = {image = "rightfoot", rotZ = 0, rotY = 0},
			hold        = {image = "rightfoot", flip = "TexCoordFlipMode_None"},
			holdReverse = {image = "rightfoot", flip = "TexCoordFlipMode_Y"},
			roll        = {image = "rightfoot", flip = "TexCoordFlipMode_None"},
			rollReverse = {image = "reverserightfoot", flip = "TexCoordFlipMode_None"},
		},
	};

	setmetatable(buttonInfoTable, {
		__index = function(table, key, value)
			return {
				baseWidth   = 64,
				tap         = {image = "fallback", rotZ = 0, rotY = 0},
				hold        = {image = "fallback", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "fallback", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "fallback", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "fallback", flip = "TexCoordFlipMode_None"},
			};
		end
	});

	for i, button in ipairs(button_list) do
		local noteType = skin_parameters and skin_parameters.note_type or "Normal";

		local buttonInfo = buttonInfoTable[button];

		-- Button info for maniax-*
		if stepstype == "StepsType_Maniax_Single" or stepstype == "StepsType_Maniax_Double" then
			buttonInfo = {
				tap         = {image = "ring", rotZ = 0, rotY = 0},
				hold        = {image = "ring", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "ring", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "ring", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "ring", flip = "TexCoordFlipMode_None"},
			};
		end

		-- Button info for kb7-single
		if stepstype == "StepsType_Kb7_Single" then
			buttonInfo = {
				tap         = {image = "circle", rotZ = 0, rotY = 0},
				hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
			};
		end

		-- Button info for Center in techno-single5
		if (stepstype == "StepsType_Techno_Single5" or stepstype == "StepsType_Techno_Double5") and button == "Center" then
			buttonInfo = {
				tap         = {image = "circle", rotZ = 0, rotY = 0},
				hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
			};
		end

		-- Button info for note type variants
		if noteType == "Bar32" then
			buttonInfo = {
				tap         = {image = "key", rotZ = 0, rotY = 0},
				hold        = {image = "key", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "key", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "key", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "key", flip = "TexCoordFlipMode_None"},
			};
		elseif noteType == "Bar64" then
			buttonInfo = {
				tap         = {image = "scratch", rotZ = 0, rotY = 0},
				hold        = {image = "scratch", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "scratch", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "scratch", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "scratch", flip = "TexCoordFlipMode_None"},
			};
		elseif noteType == "Circle" then
			buttonInfo = {
				tap         = {image = "circle", rotZ = 0, rotY = 0},
				hold        = {image = "circle", flip = "TexCoordFlipMode_None"},
				holdReverse = {image = "circle", flip = "TexCoordFlipMode_Y"},
				roll        = {image = "circle", flip = "TexCoordFlipMode_None"},
				rollReverse = {image = "circle", flip = "TexCoordFlipMode_None"},
			};
		end

		ret[i] = Def.Sprite {
			Texture = NOTESKIN:get_path(skin_name, "_" .. buttonInfo.tap.image .. " rflash (doubleres).png");
			InitCommand = function(self)
				self:draworder(notefield_draw_order.receptor + 1):blend("BlendMode_Add")
					:rotationz(buttonInfo.tap.rotZ):rotationy(buttonInfo.tap.rotY):diffusealpha(0);
			end,
			WidthSetCommand = function(self, param)
				param.column:set_layer_fade_type(self, "FieldLayerFadeType_Receptor")
			end,
			BeatUpdateCommand = function(self, param)
				if param.pressed then
					self:finishtweening():zoom(1):diffusealpha(0.6);
				elseif param.lifted then
					self:finishtweening():decelerate(0.12):diffusealpha(0):zoom(1.2);
				end
			end,
		};
	end
	return ret;
end
