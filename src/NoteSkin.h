#ifndef NEW_SKIN_H
#define NEW_SKIN_H

#include <unordered_set>

#include "Actor.h"
#include "ActorFrame.h"
#include "AutoActor.h"
#include "NoteTypes.h"
#include "RageTexture.h"
#include "RageUtil.hpp"

// Receptors and explosions are full actors.  There are a fixed number of
// them, and that number is relatively small.  Their update functions will
// be called each frame.
// Taps are actors that occur at a single point in time.  One is made for
// each NoteSkinTapPart and NoteSkinTapOptionalPart, and that one is reused
// whenever a tap of that part is needed.
// Everything in Tap and Hold is considered quantizable.  They get a
// state map to control what part of their texture is used at a given
// quantization and beat.
// Holds are loaded by the tap loader, so there isn't a separate enum entry
// for holds.
// Holds must be stretched over a period, so they are not actors at all.
// Instead, they only have 6 textures: the two caps and the body, in active
// and inactive states.  These textures are then rendered to generated
// quads.

enum NoteSkinTapPart
{
	// These tap parts must be provided by the noteskin.  If they are absent,
	// it is an error.
	NSTP_Tap,
	NSTP_Mine,
	NSTP_Lift,
	NUM_NoteSkinTapPart,
	NoteSkinTapPart_Invalid
};
std::string const NoteSkinTapPartToString(NoteSkinTapPart nsp);
LuaDeclareType(NoteSkinTapPart);

enum NoteSkinTapOptionalPart
{
	// These tap parts are optional.  If none of them exist, nothing is used.
	// If HoldHead exists and RollHead does not, HoldHead is used when a
	// RollHead is needed.
	NSTOP_HoldHead,
	NSTOP_HoldTail,
	NSTOP_RollHead,
	NSTOP_RollTail,
	NSTOP_CheckpointHead,
	NSTOP_CheckpointTail,
	NUM_NoteSkinTapOptionalPart,
	NoteSkinTapOptionalPart_Invalid
};
std::string const NoteSkinTapOptionalPartToString(NoteSkinTapOptionalPart nsp);
LuaDeclareType(NoteSkinTapOptionalPart);

enum NoteSkinHoldPart
{
	NSHP_Top,
	NSHP_Body,
	NSHP_Bottom,
	NUM_NoteSkinHoldPart,
	NoteSkinHoldPart_Invalid
};
std::string const NoteSkinHoldPartToString(NoteSkinHoldPart nsp);
LuaDeclareType(NoteSkinHoldPart);

// There are three modes for playerizing notes for routine mode.
// NPM_Off is for not playerizing at all.
// NPM_Mask uses the color mask in the noteskin.
// NPM_Quanta uses the quanta in the noteskin as per-player notes.
enum NotePlayerizeMode
{
	NPM_Off,
	NPM_Mask,
	NPM_Quanta,
	NUM_NotePlayerizeMode,
	NotePlayerizeMode_Invalid
};
std::string const NotePlayerizeModeToString(NotePlayerizeMode npm);
LuaDeclareType(NotePlayerizeMode);

struct NoteSkinLoader;
struct TimingSource;

struct QuantizedStateMap
{
	static const size_t max_quanta= 256;
	static const size_t max_states= 256;
	// A QuantizedStateMap has a list of the quantizations the noteskin has.
	// A quantization occurs a fixed integer number of times per beat and has a
	// few states for its animation.
	struct QuantizedStates
	{
		size_t per_beat;
		std::vector<size_t> states;
	};

	QuantizedStateMap()
	{
		clear();
	}

	QuantizedStates const& calc_quantization(double quantization) const
	{
		// Real world use case for solving the fizzbuzz problem.  Find the
		// largest factor for a number from the entries in a short list.
		size_t beat_part= static_cast<size_t>((quantization * m_parts_per_beat) + .5);
		for(auto&& quantum : m_quanta)
		{
			size_t spacing= static_cast<size_t>(m_parts_per_beat / quantum.per_beat);
			if(spacing * quantum.per_beat != m_parts_per_beat)
			{
				// This quantum is finer than what is supported by the parts per
				// beat.  Skipping it allows a noteskin author to twiddle the
				// quantization of the skin by changing the parts per beat without
				// changing the list of quantizations.
				continue;
			}
			if(beat_part % spacing == 0)
			{
				return quantum;
			}
		}
		return m_quanta.back();
	}
	size_t calc_frame(QuantizedStates const& quantum, double quantization,
		double beat, bool vivid) const
	{
		size_t frame_index= static_cast<size_t>(
			floor(((vivid ? quantization : 0.0) + beat) * quantum.states.size()))
			% quantum.states.size();
		return quantum.states[frame_index];
	}
	size_t calc_state(double quantization, double beat, bool vivid) const
	{
		QuantizedStates const& quantum= calc_quantization(quantization);
		return calc_frame(quantum, quantization, beat, vivid);
	}
	size_t calc_player_state(size_t pn, double beat, bool vivid) const
	{
		QuantizedStates const& quantum= m_quanta[pn%m_quanta.size()];
		return calc_frame(quantum, 0.0, beat, vivid);
	}
	bool load_from_lua(lua_State* L, int index, std::string& insanity_diagnosis);
	void swap(QuantizedStateMap& other)
	{
		size_t tmp= m_parts_per_beat;
		m_parts_per_beat= other.m_parts_per_beat;
		other.m_parts_per_beat= tmp;
		m_quanta.swap(other.m_quanta);
	}
	void clear()
	{
		m_parts_per_beat= 1;
		m_quanta.resize(1);
		m_quanta[0]= {1, {1}};
	}
private:
	size_t m_parts_per_beat;
	std::vector<QuantizedStates> m_quanta;
};

struct QuantizedTextureMap
{
	static const size_t max_quanta= 256;

	struct TextureQuanta
	{
		size_t per_beat;
		float trans_x;
		float trans_y;
	};

	QuantizedTextureMap()
	{
		clear();
	}

	TextureQuanta const& calc_quantization(double quantization) const
	{
		size_t beat_part= static_cast<size_t>((quantization * m_parts_per_beat) + .5);
		for(auto&& quantum : m_quanta)
		{
			size_t spacing= m_parts_per_beat / quantum.per_beat;
			if(spacing * quantum.per_beat != m_parts_per_beat)
			{
				continue;
			}
			if(beat_part % spacing == 0)
			{
				return quantum;
			}
		}
		return m_quanta.back();
	}

	void calc_trans(double quantization, double beat, bool vivid, float& trans_x, float& trans_y, float& seconds_in)
	{
		TextureQuanta const& quantum= calc_quantization(quantization);
		trans_x= quantum.trans_x;
		trans_y= quantum.trans_y;
		seconds_in= vivid ? beat+quantization : beat;
	}
	void calc_player_trans(size_t pn, double beat, float& trans_x,
		float& trans_y, float& seconds_in)
	{
		TextureQuanta const& quantum= m_quanta[pn%m_quanta.size()];
		trans_x= quantum.trans_x;
		trans_y= quantum.trans_y;
		seconds_in= beat;
	}
	bool load_from_lua(lua_State* L, int index, std::string& insanity_diagnosis);
	void swap(QuantizedTextureMap& other)
	{
		std::swap(m_parts_per_beat, other.m_parts_per_beat);
		m_quanta.swap(other.m_quanta);
	}
	void clear()
	{
		m_parts_per_beat= 1;
		m_quanta.resize(1);
		m_quanta[0]= {1, 0, 0};
	}

private:
	size_t m_parts_per_beat;
	std::vector<TextureQuanta> m_quanta;
};

struct QuantizedTap
{
	void set_timing_source(TimingSource* source)
	{
		m_actor->SetTimingSource(source);
	}
	void update()
	{
		m_actor->Update(1.0f);
	}
	Actor* get_common(size_t state)
	{
		m_actor->SetState(state);
		// Return the frame and not the actor because the notefield is going to
		// apply mod transforms to it.  Returning the actor would make the mod
		// transform stomp on the rotation the noteskin supplies.
		return &m_frame;
	}
	Actor* get_quantized(double quantization, double beat, bool active)
	{
		if(m_use_texture_map)
		{
			float trans_x, trans_y, seconds_in;
			active ? m_texture_map.calc_trans(quantization, beat, m_vivid,
				trans_x, trans_y, seconds_in) :
				m_inactive_texture_map.calc_trans(quantization, beat, m_vivid,
					trans_x, trans_y, seconds_in);
			m_actor->SetSecondsIntoAnimation(seconds_in);
			m_actor->SetTextureTranslate(trans_x, trans_y);
			return &m_frame;
		}
		else
		{
			const size_t state= active ?
				m_state_map.calc_state(quantization, beat, m_vivid) :
				m_inactive_map.calc_state(quantization, beat, m_vivid);
			return get_common(state);
		}
	}
	Actor* get_playerized(size_t pn, double beat, bool active)
	{
		if(m_use_texture_map)
		{
			float trans_x, trans_y, seconds_in;
			active ? m_texture_map.calc_player_trans(pn, beat,
				trans_x, trans_y, seconds_in) :
				m_inactive_texture_map.calc_player_trans(pn, beat,
					trans_x, trans_y, seconds_in);
			m_actor->SetSecondsIntoAnimation(seconds_in);
			m_actor->SetTextureTranslate(trans_x, trans_y);
			return &m_frame;
		}
		else
		{
			const size_t state= active ?
				m_state_map.calc_player_state(pn, beat, m_vivid) :
				m_inactive_map.calc_player_state(pn, beat, m_vivid);
			return get_common(state);
		}
	}
	bool load_from_lua(lua_State* L, int index, std::string& insanity_diagnosis);
	bool m_vivid;
private:
	QuantizedStateMap m_state_map;
	QuantizedStateMap m_inactive_map;
	QuantizedTextureMap m_texture_map;
	QuantizedTextureMap m_inactive_texture_map;
	bool m_use_texture_map;
	AutoActor m_actor;
	ActorFrame m_frame;
};

enum TexCoordFlipMode
{
	TCFM_None,
	TCFM_X,
	TCFM_Y,
	TCFM_XY,
	NUM_TexCoordFlipMode,
	TexCoordFlipMode_Invalid
};
std::string const TexCoordFlipModeToString(TexCoordFlipMode tcfm);
LuaDeclareType(TexCoordFlipMode);

struct hold_part_lengths
{
	double start_note_offset;
	double end_note_offset;
	double head_pixs;
	double body_pixs;
	double tail_pixs;
};

struct QuantizedHoldRenderData
{
	QuantizedHoldRenderData() { clear(); }
	std::vector<RageTexture*> parts;
	RageTexture* mask;
	Rage::RectF const* rect;
	TexCoordFlipMode flip;
	hold_part_lengths part_lengths;
	bool texture_filtering;
	void clear()
	{
		parts.clear();
		mask= nullptr;
		rect= nullptr;
		texture_filtering= true;
	}
};

struct QuantizedHold
{
	static const size_t max_hold_layers= 32;
	QuantizedStateMap m_state_map;
	std::vector<RageTexture*> m_parts;
	TexCoordFlipMode m_flip;
	bool m_vivid;
	bool m_texture_filtering;
	hold_part_lengths m_part_lengths;
	~QuantizedHold();
	void get_common(size_t state, QuantizedHoldRenderData& ret)
	{
		for(size_t i= 0; i < m_parts.size(); ++i)
		{
			ret.parts.push_back(m_parts[i]);
			if(ret.rect == nullptr)
			{
				ret.rect= m_parts[i]->GetTextureCoordRect(state);
			}
		}
		ret.flip= m_flip;
		ret.part_lengths= m_part_lengths;
		ret.texture_filtering= m_texture_filtering;
	}
	void get_quantized(double quantization, double beat, QuantizedHoldRenderData& ret)
	{
		const size_t state= m_state_map.calc_state(quantization, beat, m_vivid);
		get_common(state, ret);
	}
	void get_playerized(size_t pn, double beat, QuantizedHoldRenderData& ret)
	{
		const size_t state= m_state_map.calc_player_state(pn, beat, m_vivid);
		get_common(state, ret);
	}
	bool load_from_lua(lua_State* L, int index, NoteSkinLoader const* load_skin, std::string& insanity_diagnosis);
};

struct NoteSkinColumn
{
	void set_timing_source(TimingSource* source);
	void update_taps();
	Actor* get_tap_actor(size_t type, double quantization, double beat, bool active, bool reverse);
	Actor* get_optional_actor(size_t type, double quantization, double beat, bool active, bool reverse);
	Actor* get_player_tap(size_t type, size_t pn, double beat, bool active, bool reverse);
	Actor* get_player_optional_tap(size_t type, size_t pn, double beat, bool active, bool reverse);
	void get_hold_render_data(TapNoteSubType sub_type,
		NotePlayerizeMode playerize_mode, size_t pn, bool active, bool reverse,
		double quantization, double beat, QuantizedHoldRenderData& data);
	double get_width() { return m_width; }
	double get_padding() { return m_padding; }
	double get_custom_x() { return m_custom_x; }
	double get_anim_mult() { return m_anim_mult; }
	double get_quantum_mult() { return m_quantum_mult; }
	float get_hold_gray_percent() { return m_hold_gray_percent; }
	bool get_use_hold_head() { return m_use_hold_heads_for_taps_on_row; }
	bool get_anim_uses_beats() { return m_anim_uses_beats; }
	bool get_use_custom_x() { return m_use_custom_x; }
	bool supports_masking()
	{
		return !(m_hold_player_masks.empty() || m_hold_reverse_player_masks.empty());
	}
	bool load_holds_from_lua(lua_State* L, int index,
		std::vector<std::vector<QuantizedHold> >& holder,
		std::string const& holds_name,
		NoteSkinLoader const* load_skin, std::string& insanity_diagnosis);
	bool load_texs_from_lua(lua_State* L, int index,
		std::vector<RageTexture*>& dest,
		std::string const& texs_name,
		NoteSkinLoader const* load_skin, std::string& insanity_diagnosis);
	bool load_from_lua(lua_State* L, int index, NoteSkinLoader const* load_skin,
		std::string& insanity_diagnosis);
	void vivid_operation(bool vivid)
	{
		for(auto&& tap_set : {&m_taps, &m_reverse_taps})
		{
			for(auto&& tap : *tap_set)
			{
				tap.m_vivid= vivid;
			}
		}
		for(auto&& tap_set : {&m_optional_taps, &m_reverse_optional_taps})
		{
			for(auto&& tap : *tap_set)
			{
				if(tap != nullptr)
				{
					tap->m_vivid= vivid;
				}
			}
		}
		for(auto&& subtype : m_holds)
		{
			for(auto&& action : subtype)
			{
				action.m_vivid= vivid;
			}
		}
		for(auto&& subtype : m_reverse_holds)
		{
			for(auto&& action : subtype)
			{
				action.m_vivid= vivid;
			}
		}
	}
	void clear_optionals()
	{
		for(auto&& tap_set : {&m_optional_taps, &m_reverse_optional_taps})
		{
			for(auto&& tap : *tap_set)
			{
				if(tap != nullptr)
				{
					Rage::safe_delete(tap);
				}
			}
		}
	}
	NoteSkinColumn()
		:m_optional_taps(NUM_NoteSkinTapOptionalPart, nullptr)
	{}
	~NoteSkinColumn()
	{
		clear_optionals();
	}
private:
	// m_taps is indexed by NoteSkinTapPart.
	std::vector<QuantizedTap> m_taps;
	std::vector<QuantizedTap> m_reverse_taps;
	// m_optional_taps is indexed by NoteSkinTapOptionalPart.
	// If an entry is null, the skin doesn't use that part.
	std::vector<QuantizedTap*> m_optional_taps;
	std::vector<QuantizedTap*> m_reverse_optional_taps;
	// Dimensions of m_holds:
	// note subtype, active/inactive.
	std::vector<std::vector<QuantizedHold> > m_holds;
	std::vector<std::vector<QuantizedHold> > m_reverse_holds;
	// m_hold_player_masks is indexed by note subtype.
	std::vector<RageTexture*> m_hold_player_masks;
	std::vector<RageTexture*> m_hold_reverse_player_masks;
	double m_width;
	double m_padding;
	double m_custom_x;
	double m_hold_gray_percent;
	// m_anim_mult and m_quantization_mult are used to control how many beats
	// the animation and quantization are spread over.  The noteskin supplies a
	// number of beats, which is converted to its reciprocal.  The reciprocal
	// is used because multiplication is faster than division.
	double m_anim_mult;
	double m_quantum_mult;
	bool m_anim_uses_beats;
	bool m_use_hold_heads_for_taps_on_row;
	bool m_use_custom_x;
};

struct NoteSkinLayer
{
	bool load_from_lua(lua_State* L, int index, size_t columns, std::string& insanity_diagnosis);
	// The actors are public so that the NoteFieldColumns can go through and
	// take ownership of the actors after loading.
	std::vector<Actor*> m_actors;
};

struct NoteSkinData
{
	static const size_t max_columns= 256;
	NoteSkinData();
	void swap(NoteSkinData& other);
	NoteSkinColumn* get_column(size_t column)
	{
		if(column >= m_columns.size())
		{
			return nullptr;
		}
		return &m_columns[column];
	}
	size_t num_columns() { return m_columns.size(); }
	bool load_taps_from_lua(lua_State* L, int index, size_t columns,
		NoteSkinLoader const* load_skin, std::string& insanity_diagnosis);
	bool loaded_successfully() const { return m_loaded; }

	// The layers are public so that the NoteFieldColumns can go through and
	// take ownership of the actors after loading.
	std::vector<NoteSkinLayer> m_layers;
	std::vector<Rage::Color> m_player_colors;
private:
	std::vector<NoteSkinColumn> m_columns;
	LuaReference m_skin_parameters;
	bool m_loaded;
};

struct NoteSkinLoader
{
	static const size_t max_layers= 16;
	NoteSkinLoader()
		:m_supports_all_buttons(false)
	{}
	std::string const& get_name() const
	{
		return m_skin_name;
	}
	std::string const& get_fallback_name() const
	{
		return m_fallback_skin_name;
	}
	std::string const& get_load_path() const
	{
		return m_load_path;
	}
	void swap(NoteSkinLoader& other);
	bool load_from_file(std::string const& path);
	bool load_from_lua(lua_State* L, int index, std::string const& name,
		std::string const& path, std::string& insanity_diagnosis);
	bool supports_needed_buttons(StepsType stype) const;
	bool push_loader_function(lua_State* L, std::string const& loader);
	bool load_layer_set_into_data(lua_State* L, LuaReference& skin_params,
		int button_list_index, int stype_index,
		size_t columns, std::vector<std::string> const& loader_set,
		std::vector<NoteSkinLayer>& dest, std::string& insanity_diagnosis);
	bool load_into_data(StepsType stype, LuaReference& skin_params,
		NoteSkinData& dest, std::string& insanity_diagnosis);
	void sanitize_skin_parameters(lua_State* L, LuaReference& params);
	void push_skin_parameter_info(lua_State* L) const;
	void push_skin_parameter_defaults(lua_State* L) const;
private:
	void recursive_sanitize_skin_parameters(lua_State* L,
		std::unordered_set<void const*>& visited_tables, int curr_depth,
		int curr_param_set_info, int curr_param_set_defaults,
		int curr_param_set_dest);
	std::string m_skin_name;
	std::string m_fallback_skin_name;
	std::string m_load_path;
	std::string m_notes_loader;
	std::vector<std::string> m_layer_loaders;
	std::vector<Rage::Color> m_player_colors;
	std::unordered_set<std::string> m_supported_buttons;
	LuaReference m_skin_parameters;
	LuaReference m_skin_parameter_info;
	bool m_supports_all_buttons;
};

#endif
