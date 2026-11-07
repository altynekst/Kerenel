#include <base/log.h>
#include <base/math.h>
#include <base/system.h>

#include <engine/font_icons.h>
#include <engine/graphics.h>
#include <engine/shared/config.h>
#include <engine/shared/localization.h>
#include <engine/shared/protocol7.h>
#include <engine/storage.h>
#include <engine/textrender.h>
#include <engine/updater.h>

#include <generated/protocol.h>

#include <game/client/animstate.h>
#include <game/client/components/chat.h>
#include <game/client/components/countryflags.h>
#include <game/client/components/menu_background.h>
#include <game/client/components/menus.h>
#include <game/client/components/skins.h>
#include <game/client/components/sounds.h>
#include <game/client/components/tclient/zz_theme.h>
#include <game/client/gameclient.h>
#include <game/client/skin.h>
#include <game/client/ui.h>
#include <game/client/ui_listbox.h>
#include <game/client/ui_scrollregion.h>
#include <game/client/zz_visual_defaults.h>
#include <game/localization.h>

#include <array>
#include <chrono>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std::chrono_literals;

void CMenus::RenderSettingsKernelNet(CUIRect MainView)
{
	static int s_SubTab = 0;
	static CButtonContainer s_aSubTabs[7] = {};
	static int64_t s_SubTabAnimStart = 0;
	static int s_SubTabAnimFrom = 0;
	static int s_SubTabAnimTo = 0;
	const SZZThemePalette ActiveTheme = GetZZThemePalette(
		g_Config.m_ClZzTheme,
		color_cast<ColorRGBA>(ColorHSLA(g_Config.m_UiColor, true)));

	CUIRect TabBar, Button;
	MainView.HSplitTop(26.0f, &TabBar, &MainView);
	const float TabWidth = TabBar.w / 7.0f;
	const char *apTabNames[] = {Localize("Aimbot"), Localize("Gores"),
		Localize("Misc"), Localize("Fly"),
		Localize("FNG"), Localize("Visuals"),
		"Recording"};
	if(s_SubTab >= 7)
		s_SubTab = 0;
	for(int i = 0; i < 7; i++)
	{
		TabBar.VSplitLeft(TabWidth, &Button, &TabBar);
		const int Corners = i == 0 ? IGraphics::CORNER_L : i == 6 ? IGraphics::CORNER_R :
									    IGraphics::CORNER_NONE;
		if(DoButton_MenuTab(&s_aSubTabs[i], apTabNames[i], s_SubTab == i, &Button,
			   Corners, nullptr, nullptr, nullptr, nullptr, 4.0f))
		{
			s_SubTabAnimFrom = s_SubTab;
			s_SubTabAnimTo = i;
			s_SubTabAnimStart = time_get();
			s_SubTab = i;
		}
	}
	MainView.HSplitTop(10.0f, nullptr, &MainView);

	CUIRect Label, TitleLabel, ShortcutLabel;
	MainView.HSplitTop(38.0f, &Label, &MainView);
	Label.Draw(ActiveTheme.m_Panel.WithAlpha(0.76f), IGraphics::CORNER_ALL, 8.0f);
	Label.Margin(10.0f, &Label);
	Label.VSplitRight(150.0f, &TitleLabel, &ShortcutLabel);
	Ui()->DoLabel(&TitleLabel, Localize("KernelNet settings"), 20.0f,
		TEXTALIGN_ML);
	Ui()->DoLabel(&ShortcutLabel, apTabNames[s_SubTab], 14.0f, TEXTALIGN_MR);
	MainView.HSplitTop(10.0f, nullptr, &MainView);

	const int64_t Now = time_get();
	const float AnimDuration = 0.16f;
	const float AnimDurationTicks = AnimDuration * (float)time_freq();
	float AnimT = 1.0f;
	if(s_SubTabAnimStart != 0 && AnimDurationTicks > 0.0f)
	{
		AnimT = std::clamp((float)(Now - s_SubTabAnimStart) / AnimDurationTicks,
			0.0f, 1.0f);
		if(AnimT >= 1.0f)
			s_SubTabAnimStart = 0;
	}
	const float Ease = AnimT * AnimT * (3.0f - 2.0f * AnimT);
	const float FadeAlpha = 0.72f + 0.28f * Ease;
	const float SlideDir = (float)((s_SubTabAnimTo > s_SubTabAnimFrom) -
				       (s_SubTabAnimTo < s_SubTabAnimFrom));
	MainView.x += (1.0f - Ease) * SlideDir * 22.0f;
	ColorRGBA OldTextColor = TextRender()->DefaultTextColor();
	TextRender()->TextColor(ActiveTheme.m_Text.WithMultipliedAlpha(FadeAlpha));

	auto DoFeaturePanel = [&](CUIRect &View, float Height, const char *pTitle,
				      CUIRect &Inner) {
		CUIRect Panel, Header, Accent, Title;
		View.HSplitTop(Height + 34.0f, &Panel, &View);
		Panel.Draw(ActiveTheme.m_Panel.WithAlpha(0.82f), IGraphics::CORNER_ALL,
			7.0f);
		Panel.HSplitTop(30.0f, &Header, &Inner);
		Header.VSplitLeft(3.0f, &Accent, &Title);
		Accent.y += 7.0f;
		Accent.h -= 14.0f;
		Accent.Draw(ActiveTheme.m_Accent, IGraphics::CORNER_ALL, 1.5f);
		Title.Margin(10.0f, &Title);
		Ui()->DoLabel(&Title, pTitle, 15.0f, TEXTALIGN_ML);
		Inner.Margin(10.0f, &Inner);
		View.HSplitTop(10.0f, nullptr, &View);
	};

	if(s_SubTab == 0)
	{
		const float LineSize = 20.0f;
		CUIRect Left, Right;
		MainView.VSplitMid(&Left, &Right, 20.0f);

		// Left: Aimbot
		{
			CUIRect Inner;
			DoFeaturePanel(Left, 210.0f, Localize("Aimbot"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAimbotEnabled, Localize("Aimbot enabled?"),
				&g_Config.m_ClZzAimbotEnabled, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzAimbotFov,
				&g_Config.m_ClZzAimbotFov, &Button,
				Localize("Aimbot FOV"), 1, 180);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAimbotIgnoreFriends, Localize("ignore friends"),
				&g_Config.m_ClZzAimbotIgnoreFriends, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAimbotSilent, Localize("silent"),
				&g_Config.m_ClZzAimbotSilent, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAimbotShowFov, Localize("show FOV"),
				&g_Config.m_ClZzAimbotShowFov, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAimbotPreferFrozen, Localize("prefer frozen hook"),
				&g_Config.m_ClZzAimbotPreferFrozen, &Inner, LineSize);

			Inner.HSplitTop(5.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Aimbot toggle keybind"), 14.0f,
				TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			static CButtonContainer s_AimbotKeyReader;
			static CButtonContainer s_AimbotKeyClear;
			CBindSlot CurrentBindAimbot((int)g_Config.m_ClZzAimbotKey,
				(int)g_Config.m_ClZzAimbotMod);
			const auto ResAimbot = GameClient()->m_KeyBinder.DoKeyReader(
				&s_AimbotKeyReader, &s_AimbotKeyClear, &Button, CurrentBindAimbot,
				false);
			if(!ResAimbot.m_Aborted &&
				(ResAimbot.m_Bind.m_Key != CurrentBindAimbot.m_Key ||
					ResAimbot.m_Bind.m_ModifierMask !=
						CurrentBindAimbot.m_ModifierMask))
			{
				if(CurrentBindAimbot.m_Key >= KEY_FIRST &&
					CurrentBindAimbot.m_Key < KEY_LAST)
					GameClient()->m_Binds.Bind(CurrentBindAimbot.m_Key, "", false,
						CurrentBindAimbot.m_ModifierMask);
				g_Config.m_ClZzAimbotKey = ResAimbot.m_Bind.m_Key;
				g_Config.m_ClZzAimbotMod = ResAimbot.m_Bind.m_ModifierMask;
				if(ResAimbot.m_Bind.m_Key >= KEY_FIRST &&
					ResAimbot.m_Bind.m_Key < KEY_LAST)
					GameClient()->m_Binds.Bind(ResAimbot.m_Bind.m_Key,
						"+zz_aimbot_toggle", false,
						ResAimbot.m_Bind.m_ModifierMask);
			}
		}

		// Hookable blocks and the hold-to-hook tile bind belong to Aimbot.
		{
			CUIRect Inner;
			DoFeaturePanel(Left, 190.0f, Localize("Hook tile"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClKnHookBlocks,
				Localize("Aim at hookable blocks"), &g_Config.m_ClKnHookBlocks,
				&Inner, LineSize);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Hook tile keybind"), 14.0f,
				TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			static CButtonContainer s_SaveInBlockKeyReader;
			static CButtonContainer s_SaveInBlockKeyClear;
			CBindSlot CurrentBindSave((int)g_Config.m_ClZzSaveInBlockKey,
				(int)g_Config.m_ClZzSaveInBlockMod);
			const auto ResSave = GameClient()->m_KeyBinder.DoKeyReader(
				&s_SaveInBlockKeyReader, &s_SaveInBlockKeyClear, &Button,
				CurrentBindSave, false);
			if(!ResSave.m_Aborted && ResSave.m_Bind != CurrentBindSave)
			{
				if(CurrentBindSave.m_Key >= KEY_FIRST && CurrentBindSave.m_Key < KEY_LAST)
					GameClient()->m_Binds.Bind(CurrentBindSave.m_Key, "", false,
						CurrentBindSave.m_ModifierMask);
				g_Config.m_ClZzSaveInBlockKey = ResSave.m_Bind.m_Key;
				g_Config.m_ClZzSaveInBlockMod = ResSave.m_Bind.m_ModifierMask;
				if(ResSave.m_Bind.m_Key >= KEY_FIRST && ResSave.m_Bind.m_Key < KEY_LAST)
					GameClient()->m_Binds.Bind(ResSave.m_Bind.m_Key, "+zz_tile_aimbot",
						false, ResSave.m_Bind.m_ModifierMask);
			}
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzSaveInBlockSilent,
				Localize("Silent hook tile"), &g_Config.m_ClZzSaveInBlockSilent,
				&Inner, LineSize);
			Inner.HSplitTop(7.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize * 2.0f, &Label, &Inner);
			Ui()->DoLabel(&Label,
				Localize("Players have priority; blocks are used only when no tee can be hooked."),
				13.0f, TEXTALIGN_ML);
		}

		// Right: Auto-hit + Fly helper
		{
			CUIRect Inner;
			DoFeaturePanel(Right, 110.0f, Localize("Hammer assist"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzHammerAssistEnabled, Localize("Auto-hit enabled?"),
				&g_Config.m_ClZzHammerAssistEnabled, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzHammerAssistIgnoreFriends, Localize("ignore friends"),
				&g_Config.m_ClZzHammerAssistIgnoreFriends, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzHammerAssistSilent, Localize("silent"),
				&g_Config.m_ClZzHammerAssistSilent, &Inner, LineSize);
			Inner.HSplitTop(5.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Бьёт врага, когда вы зажимаете хаммер."),
				14.0f, TEXTALIGN_ML);
		}
	}
	else if(s_SubTab == 1)
	{
		const float LineSize = 20.0f;
		static CScrollRegion s_AvoidScrollRegion;
		static vec2 s_AvoidScrollOffset(0.0f, 0.0f);
		CScrollRegionParams ScrollParams;
		ScrollParams.m_ScrollUnit = 48.0f;
		ScrollParams.m_ScrollbarWidth = 16.0f;
		ScrollParams.m_SliderColor = ActiveTheme.m_Accent.WithAlpha(0.65f);
		ScrollParams.m_SliderColorHover = ActiveTheme.m_Accent.WithAlpha(0.85f);
		ScrollParams.m_SliderColorGrabbed = ActiveTheme.m_Accent;
		s_AvoidScrollRegion.Begin(&MainView, &s_AvoidScrollOffset, &ScrollParams);
		MainView.y += s_AvoidScrollOffset.y;
		CUIRect Left, Right;
		MainView.VSplitMid(&Left, &Right, 20.0f);

		{
            CUIRect Inner;
            DoFeaturePanel(Left, 385.0f, Localize("Avoid"), Inner);
            auto Check = [&](int &Value, const char *pName) { DoButton_CheckBoxAutoVMarginAndSet(&Value, Localize(pName), &Value, &Inner, LineSize); };
            auto Slider = [&](int &Value, const char *pName, int Min, int Max) { Inner.HSplitTop(LineSize, &Button, &Inner); Ui()->DoScrollbarOption(&Value, &Value, &Button, Localize(pName), Min, Max); };
            Check(g_Config.m_AvoidFreeze, "Enabled");
            CUIRect Modes;
            Inner.HSplitTop(LineSize, &Modes, &Inner);
            static CButtonContainer s_aAvoidModes[3];
            const char *apAvoidModes[] = {"Old", "Legit", "Blatant"};
            const float ModeWidth = Modes.w / 3.0f;
            for(int Mode = 0; Mode < 3; ++Mode)
            {
                CUIRect ModeButton;
                Modes.VSplitLeft(ModeWidth, &ModeButton, &Modes);
                if(DoButton_MenuTab(&s_aAvoidModes[Mode], Localize(apAvoidModes[Mode]), g_Config.m_AvoidAgentType == Mode, &ModeButton,
                    IGraphics::CORNER_ALL, nullptr, nullptr, nullptr, nullptr, 4.0f))
                    g_Config.m_AvoidAgentType = Mode;
            }
            Inner.HSplitTop(5.0f, nullptr, &Inner);
            if(g_Config.m_AvoidAgentType == 0)
            {
                Check(g_Config.m_ClZzAvoidFreeze, "Freeze");
                Check(g_Config.m_ClZzAvoidTele, "Teleports");
                Check(g_Config.m_ClZzAvoidKill, "Death");
                Check(g_Config.m_ClZzAvoidDirection, "Direction");
                Check(g_Config.m_ClZzAvoidJump, "Jump");
                Check(g_Config.m_ClZzAvoidHookAssist, "Hook");
                Check(g_Config.m_ClZzAvoidAim, "Aim");
                Check(g_Config.m_ClZzAvoidSilent, "Silent");
                Slider(g_Config.m_ClZzAvoidFov, "Aim FOV", 5, 360);
                Slider(g_Config.m_ClZzAvoidAngles, "Aim angles", 1, 144);
                Slider(g_Config.m_ClZzAvoidTicks, "Simulation ticks", 1, 20);
                Slider(g_Config.m_ClZzAvoidTriggerTicks, "Trigger ticks", 1, 20);
            }
            else if(g_Config.m_AvoidAgentType == 1)
            {
                Check(g_Config.m_AvoidPlayerPrediction, "Player prediction");
                Check(g_Config.m_AvoidLegitDirection, "Direction");
                Check(g_Config.m_AvoidLegitHook, "Hook");
                Check(g_Config.m_AvoidLegitDeath, "Death");
                Check(g_Config.m_AvoidLegitTeles, "Teleports");
                Check(g_Config.m_AvoidLegitUnfreeze, "Unfreeze tiles");
                Slider(g_Config.m_AvoidLegitCheckTicks, "Check ticks", 1, 50);
                Slider(g_Config.m_ClAvoidNumIterations, "Iterations", 1, 1000);
            }
            else
            {
                Check(g_Config.m_AvoidPlayerPrediction, "Player prediction");
                Check(g_Config.m_AvoidNsif, "NSIF");
                Check(g_Config.m_AvoidBlatantDirection, "Direction");
                Check(g_Config.m_AvoidBlatantHook, "Hook");
                Check(g_Config.m_AvoidBlatantDeath, "Death");
                Check(g_Config.m_AvoidBlatantTeles, "Teleports");
                Check(g_Config.m_AvoidBlatantAimbot, "Aim assist");
                Slider(g_Config.m_AvoidBlatantCheckTicks, "Check ticks", 1, 50);
                Slider(g_Config.m_AvoidKickInTicks, "Kick-in ticks", 1, 50);
            }
        }
		{
			CUIRect Inner;
			DoFeaturePanel(Left, 110.0f, Localize("Hammer gun"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzHammerGun, Localize("Hammer-gun enabled?"),
				&g_Config.m_ClZzHammerGun, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzHammerGunAutoDisableExtraWeapons,
				Localize("disable if extra weapons"),
				&g_Config.m_ClZzHammerGunAutoDisableExtraWeapons, &Inner, LineSize);
			Inner.HSplitTop(5.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(
				&Label,
				Localize("Держит пистолет, а выстрел заменяет ударом хаммера."),
				14.0f, TEXTALIGN_ML);
		}

		// Right: movement helpers.
		{
			CUIRect Inner;
			DoFeaturePanel(Right, 95.0f, Localize("Balance bot"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzBalanceBot, Localize("Balance on players"),
				&g_Config.m_ClZzBalanceBot, &Inner, LineSize);
			Inner.HSplitTop(5.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(
				&Label,
				Localize("Keeps the tee centered while standing on another tee."),
				14.0f, TEXTALIGN_ML);
		}

		if(false)
		{
			CUIRect Inner;
			DoFeaturePanel(Right, 215.0f, Localize("FentBot"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzFentBotEnabled, Localize("FentBot enabled?"),
				&g_Config.m_ClZzFentBotEnabled, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzFentBotAimFollow, Localize("follow aim"),
				&g_Config.m_ClZzFentBotAimFollow, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzFentBotShowPath, Localize("show path"),
				&g_Config.m_ClZzFentBotShowPath, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzFentBotHorizonTicks,
				&g_Config.m_ClZzFentBotHorizonTicks, &Button,
				Localize("Horizon ticks"), 8, 80);
			Inner.HSplitTop(5.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Waypoint keybind"), 14.0f, TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			static CButtonContainer s_FentBotWpKeyReader;
			static CButtonContainer s_FentBotWpKeyClear;
			CBindSlot CurrentBindWp((int)g_Config.m_ClZzFentBotWaypointKey,
				(int)g_Config.m_ClZzFentBotWaypointMod);
			const auto ResWp = GameClient()->m_KeyBinder.DoKeyReader(
				&s_FentBotWpKeyReader, &s_FentBotWpKeyClear, &Button, CurrentBindWp,
				false);
			if(!ResWp.m_Aborted &&
				(ResWp.m_Bind.m_Key != CurrentBindWp.m_Key ||
					ResWp.m_Bind.m_ModifierMask != CurrentBindWp.m_ModifierMask))
			{
				if(CurrentBindWp.m_Key >= KEY_FIRST && CurrentBindWp.m_Key < KEY_LAST)
					GameClient()->m_Binds.Bind(CurrentBindWp.m_Key, "", false,
						CurrentBindWp.m_ModifierMask);
				g_Config.m_ClZzFentBotWaypointKey = ResWp.m_Bind.m_Key;
				g_Config.m_ClZzFentBotWaypointMod = ResWp.m_Bind.m_ModifierMask;
				if(ResWp.m_Bind.m_Key >= KEY_FIRST && ResWp.m_Bind.m_Key < KEY_LAST)
					GameClient()->m_Binds.Bind(ResWp.m_Bind.m_Key, "+zz_fentbot_waypoint",
						false, ResWp.m_Bind.m_ModifierMask);
			}
			Inner.HSplitTop(5.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(
				&Label,
				Localize(
					"Бот для gores: планирует движение/прыжки/хук и избегает фриза."),
				14.0f, TEXTALIGN_ML);
		}
		CUIRect ContentRect = MainView;
		ContentRect.h = std::max(Left.y, Right.y) - MainView.y;
		s_AvoidScrollRegion.AddRect(ContentRect);
		s_AvoidScrollRegion.End();
	}
	else if(s_SubTab == 2)
	{
		const float LineSize = 20.0f;
		static CScrollRegion s_MiscScrollRegion;
		static vec2 s_MiscScrollOffset(0.0f, 0.0f);
		CScrollRegionParams ScrollParams;
		ScrollParams.m_ScrollUnit = 48.0f;
		ScrollParams.m_ScrollbarWidth = 16.0f;
		ScrollParams.m_ScrollbarMargin = 3.0f;
		ScrollParams.m_Flags = CScrollRegionParams::FLAG_CONTENT_STATIC_WIDTH;
		ScrollParams.m_RailBgColor = ActiveTheme.m_PanelHover.WithAlpha(0.55f);
		ScrollParams.m_SliderColor = ActiveTheme.m_Accent.WithAlpha(0.62f);
		ScrollParams.m_SliderColorHover = ActiveTheme.m_Accent.WithAlpha(0.82f);
		ScrollParams.m_SliderColorGrabbed = ActiveTheme.m_Accent;
		s_MiscScrollRegion.Begin(&MainView, &s_MiscScrollOffset, &ScrollParams);
		MainView.y += s_MiscScrollOffset.y;
		CUIRect LeftView, RightView;
		MainView.VSplitMid(&LeftView, &RightView, 20.0f);

		// OpenClient laser-unfreeze settings.
		{
			CUIRect Inner;
			DoFeaturePanel(LeftView, 260.0f, Localize("Laser Unfreeze"), Inner);
            DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_KrxUnfreezeBotAdvanced, Localize("Advanced settings"), &g_Config.m_KrxUnfreezeBotAdvanced, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_KrxUnfreezeBot, Localize("Auto-unfreeze"),
				&g_Config.m_KrxUnfreezeBot, &Inner, LineSize);

			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_KrxUnfreezeBotFov,
				&g_Config.m_KrxUnfreezeBotFov, &Button, Localize("FOV"), 1, 315);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_KrxUnfreezeBotPoints,
				&g_Config.m_KrxUnfreezeBotPoints, &Button, Localize("Angles"), 1,
				144);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_KrxUnfreezeBotTicks,
				&g_Config.m_KrxUnfreezeBotTicks, &Button, Localize("Ticks"), 1, 100);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_KrxUnfreezeBotCurDirTicks,
				&g_Config.m_KrxUnfreezeBotCurDirTicks, &Button,
				Localize("Current aim ticks"), 1, 100);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_KrxUnfreezeBotSilent, Localize("Silent"),
				&g_Config.m_KrxUnfreezeBotSilent, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_KrxUnfreezeBotEsp, Localize("Show attempt"),
				&g_Config.m_KrxUnfreezeBotEsp, &Inner, LineSize);
		}

		// Right: utility toggles.
		{
			CUIRect Inner;
			DoFeaturePanel(RightView, 305.0f, Localize("Utility"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_KrxHookRide, Localize("Hook ride"), &g_Config.m_KrxHookRide, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAutoAled, Localize("Auto aled enabled?"),
				&g_Config.m_ClZzAutoAled, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzAutoShotgun, Localize("Auto-shotgun"),
				&g_Config.m_ClZzAutoShotgun, &Inner, LineSize);


			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzForceZoom, Localize("Force zoom"),
				&g_Config.m_ClZzForceZoom, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzFakePing,
				&g_Config.m_ClZzFakePing, &Button,
				Localize("Fake ping (ms, 0=off)"), 0, 999);
			Inner.HSplitTop(8.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Companion"), 15.0f, TEXTALIGN_ML);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzCompanionEnabled, Localize("Companion enabled"),
				&g_Config.m_ClZzCompanionEnabled, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzCompanionFollowTiles,
				&g_Config.m_ClZzCompanionFollowTiles, &Button,
				Localize("Follow distance (tiles)"), 0, 30);
			Inner.HSplitTop(6.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(
				&Label,
				Localize(
					"Auto-unfreeze заранее подбирает рикошет лазера перед фризом."),
				14.0f, TEXTALIGN_ML);
		}

		{
			CUIRect Inner;
			DoFeaturePanel(RightView, 95.0f, Localize("Click GUI"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzClickGui, Localize("Show ClickGUI"),
				&g_Config.m_ClZzClickGui, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Open / close direct key"), 14.0f,
				TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			static CButtonContainer s_ClickGuiKeyReader;
			static CButtonContainer s_ClickGuiKeyClear;
			const CBindSlot CurrentClickGuiBind(g_Config.m_ClZzClickGuiKey,
				g_Config.m_ClZzClickGuiMod);
			const auto Result = GameClient()->m_KeyBinder.DoKeyReader(
				&s_ClickGuiKeyReader, &s_ClickGuiKeyClear, &Button,
				CurrentClickGuiBind, false);
			if(!Result.m_Aborted && Result.m_Bind != CurrentClickGuiBind)
			{
				g_Config.m_ClZzClickGuiKey = Result.m_Bind.m_Key;
				g_Config.m_ClZzClickGuiMod = Result.m_Bind.m_ModifierMask;
			}
		}

		{
			CUIRect Inner;
			DoFeaturePanel(LeftView, 205.0f, Localize("Combat and prediction"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClKnWeaponPrediction,
				Localize("Weapon prediction"), &g_Config.m_ClKnWeaponPrediction,
				&Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClKnWatcherList,
				Localize("Spectators and observers"), &g_Config.m_ClKnWatcherList,
				&Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClKnGrenadeSave,
				Localize("Auto grenade save"), &g_Config.m_ClKnGrenadeSave,
				&Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzFlyRide,
				Localize("Hook drive"), &g_Config.m_ClZzFlyRide, &Inner, LineSize);
		}
		{
			CUIRect Inner;
			DoFeaturePanel(RightView, 115.0f, Localize("Player tools"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClKnIdentitySteal,
				Localize("Steal nearest name and skin."), &g_Config.m_ClKnIdentitySteal,
				&Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClKnEmojiButton,
				Localize("Emoji button in chat"), &g_Config.m_ClKnEmojiButton,
				&Inner, LineSize);
		}

		CUIRect ContentEnd = MainView;
		ContentEnd.y = maximum(LeftView.y, RightView.y);
		ContentEnd.h = 0.0f;
		s_MiscScrollRegion.AddRect(ContentEnd);
		s_MiscScrollRegion.End();
	}
	else if(s_SubTab == 3)
	{
		const float LineSize = 20.0f;
		CUIRect Inner;
		DoFeaturePanel(MainView, 250.0f, Localize("Fly"), Inner);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFlyHelperEnabled, Localize("Fly enabled?"),
			&g_Config.m_ClZzFlyHelperEnabled, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzHookFly,
			Localize("Hook fly"), &g_Config.m_ClZzHookFly, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzTripleFly,
			Localize("Triple fly"), &g_Config.m_ClZzTripleFly, &Inner, LineSize);
		Inner.HSplitTop(LineSize, &Button, &Inner);
		Ui()->DoScrollbarOption(&g_Config.m_ClZzFlyHelperHitTiles,
			&g_Config.m_ClZzFlyHelperHitTiles, &Button,
			Localize("Hit range x0.1 tiles"), 1, 80);
		Inner.HSplitTop(LineSize, &Button, &Inner);
		Ui()->DoScrollbarOption(&g_Config.m_ClZzFlyHelperHitMs,
			&g_Config.m_ClZzFlyHelperHitMs, &Button,
			Localize("Dummy hit delay ms"), 0, 1000);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFlyHelperRelaxed, Localize("Relaxed server mode"),
			&g_Config.m_ClZzFlyHelperRelaxed, &Inner, LineSize);
		Inner.HSplitTop(10.0f, nullptr, &Inner);
		Inner.HSplitTop(LineSize, &Label, &Inner);
		Ui()->DoLabel(&Label, Localize("Работает напрямую с подключенным дамми."),
			14.0f, TEXTALIGN_ML);
	}
	else if(false)
	{
		const float LineSize = 20.0f;
		const float MarginSmall = 5.0f;
		CUIRect Left, Right;
		MainView.VSplitMid(&Left, &Right, 20.0f);

		// Left: quick mode + KoG policy behavior.
		{
			CUIRect Inner;
			DoFeaturePanel(Left, 290.0f, Localize("AI controller"), Inner);
			int DerivedAiQuickMode = 0;
			if(g_Config.m_ClZzKogAiEnabled)
				DerivedAiQuickMode = 5;
			else if(g_Config.m_ClZzCompanionEnabled &&
				g_Config.m_ClZzCompanionPyInfer)
				DerivedAiQuickMode = 4;
			else if(g_Config.m_ClZzFentBotEnabled && g_Config.m_ClZzFentBotAiInfer)
				DerivedAiQuickMode = 3;
			else if(g_Config.m_ClZzFentBotEnabled && g_Config.m_ClZzFentBotPyInfer)
				DerivedAiQuickMode = 2;
			else if(g_Config.m_ClZzFentBotEnabled)
				DerivedAiQuickMode = 1;

			static int s_AiQuickModeUiAi = 0;
			if(!Ui()->MouseButton(0))
				s_AiQuickModeUiAi = DerivedAiQuickMode;
			Inner.HSplitTop(LineSize, &Button, &Inner);
			if(Ui()->DoScrollbarOption(&s_AiQuickModeUiAi, &s_AiQuickModeUiAi,
				   &Button, Localize("AI quick mode"), 0, 5))
			{
				char aCmd[64];
				str_format(aCmd, sizeof(aCmd), "zz_ai_mode %d", s_AiQuickModeUiAi);
				Console()->ExecuteLine(aCmd, IConsole::CLIENT_ID_UNSPECIFIED);
			}
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label,
				Localize("0=off 1=fentbot 2=python 3=bc 4=companion 5=kog"),
				14.0f, TEXTALIGN_ML);

			Inner.HSplitTop(8.0f, nullptr, &Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzKogAiEnabled, Localize("KoG AI enabled?"),
				&g_Config.m_ClZzKogAiEnabled, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzKogAiLearning, Localize("Online learning"),
				&g_Config.m_ClZzKogAiLearning, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzKogAiLog, Localize("Verbose logs"),
				&g_Config.m_ClZzKogAiLog, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzKogAiDifficulty,
				&g_Config.m_ClZzKogAiDifficulty, &Button,
				Localize("Difficulty"), 1, 3);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzKogAiReactionTime,
				&g_Config.m_ClZzKogAiReactionTime, &Button,
				Localize("Reaction ms"), 20, 500);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzKogAiAggression,
				&g_Config.m_ClZzKogAiAggression, &Button,
				Localize("Aggression"), 0, 100);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzKogAiStyle,
				&g_Config.m_ClZzKogAiStyle, &Button,
				Localize("Style"), 0, 2);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label,
				Localize("Style: 0=aggressive 1=careful 2=balanced"), 14.0f,
				TEXTALIGN_ML);
		}

		// Right: model paths + commands.
		{
			CUIRect Inner;
			DoFeaturePanel(Right, 290.0f, Localize("Models and actions"), Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("KoG model file (relative to save dir)"),
				14.0f, TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			static CLineInput s_KogModelPath;
			s_KogModelPath.SetBuffer(g_Config.m_ClZzKogAiModel,
				sizeof(g_Config.m_ClZzKogAiModel));
			s_KogModelPath.SetEmptyText("kog_ai/model.bin");
			Ui()->DoEditBox(&s_KogModelPath, &Button, 12.0f);

			Inner.HSplitTop(8.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label,
				Localize("FentBot model file (relative to save dir)"),
				14.0f, TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			static CLineInput s_FentModelPathAi;
			s_FentModelPathAi.SetBuffer(g_Config.m_ClZzFentBotAiModel,
				sizeof(g_Config.m_ClZzFentBotAiModel));
			s_FentModelPathAi.SetEmptyText("fentbot/model.bin");
			Ui()->DoEditBox(&s_FentModelPathAi, &Button, 12.0f);

			Inner.HSplitTop(8.0f, nullptr, &Inner);
			CUIRect Row;
			Inner.HSplitTop(LineSize, &Row, &Inner);
			static CButtonContainer s_KogStatus;
			if(DoButtonLineSize_Menu(&s_KogStatus, Localize("KoG: status"), 0, &Row,
				   LineSize, false, 0, IGraphics::CORNER_ALL, 5.0f,
				   0.0f, ColorRGBA(0.0f, 0.0f, 0.0f, 0.25f)))
				Console()->ExecuteLine("zz_kog_ai_status",
					IConsole::CLIENT_ID_UNSPECIFIED);

			Inner.HSplitTop(MarginSmall, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Row, &Inner);
			static CButtonContainer s_KogLoad;
			if(DoButtonLineSize_Menu(&s_KogLoad, Localize("KoG: load model"), 0,
				   &Row, LineSize, false, 0, IGraphics::CORNER_ALL,
				   5.0f, 0.0f,
				   ColorRGBA(0.0f, 0.0f, 0.0f, 0.25f)))
			{
				char aCmd[512];
				str_format(aCmd, sizeof(aCmd), "zz_kog_ai_load_model %s",
					g_Config.m_ClZzKogAiModel);
				Console()->ExecuteLine(aCmd, IConsole::CLIENT_ID_UNSPECIFIED);
			}

			Inner.HSplitTop(MarginSmall, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Row, &Inner);
			static CButtonContainer s_KogSave;
			if(DoButtonLineSize_Menu(&s_KogSave, Localize("KoG: save model"), 0,
				   &Row, LineSize, false, 0, IGraphics::CORNER_ALL,
				   5.0f, 0.0f,
				   ColorRGBA(0.0f, 0.0f, 0.0f, 0.25f)))
			{
				char aCmd[512];
				str_format(aCmd, sizeof(aCmd), "zz_kog_ai_save_model %s",
					g_Config.m_ClZzKogAiModel);
				Console()->ExecuteLine(aCmd, IConsole::CLIENT_ID_UNSPECIFIED);
			}

			Inner.HSplitTop(MarginSmall, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Row, &Inner);
			static CButtonContainer s_KogExport;
			if(DoButtonLineSize_Menu(&s_KogExport, Localize("KoG: export dataset"),
				   0, &Row, LineSize, false, 0,
				   IGraphics::CORNER_ALL, 5.0f, 0.0f,
				   ColorRGBA(0.0f, 0.0f, 0.0f, 0.25f)))
				Console()->ExecuteLine("zz_kog_ai_export_dataset kog_ai/dataset.bin",
					IConsole::CLIENT_ID_UNSPECIFIED);

			Inner.HSplitTop(MarginSmall, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Row, &Inner);
			static CButtonContainer s_KogTrainOffline;
			if(DoButtonLineSize_Menu(&s_KogTrainOffline,
				   Localize("KoG: train offline"), 0, &Row,
				   LineSize, false, 0, IGraphics::CORNER_ALL, 5.0f,
				   0.0f, ColorRGBA(0.0f, 0.0f, 0.0f, 0.25f)))
				Console()->ExecuteLine("zz_kog_ai_train_offline kog_ai/dataset.bin 12",
					IConsole::CLIENT_ID_UNSPECIFIED);

			Inner.HSplitTop(MarginSmall, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Row, &Inner);
			static CButtonContainer s_FentLoadAi;
			if(DoButtonLineSize_Menu(&s_FentLoadAi, Localize("FentBot: load model"),
				   0, &Row, LineSize, false, 0,
				   IGraphics::CORNER_ALL, 5.0f, 0.0f,
				   ColorRGBA(0.0f, 0.0f, 0.0f, 0.25f)))
			{
				char aCmd[512];
				str_format(aCmd, sizeof(aCmd), "zz_fentbot_load_model %s",
					g_Config.m_ClZzFentBotAiModel);
				Console()->ExecuteLine(aCmd, IConsole::CLIENT_ID_UNSPECIFIED);
			}
		}
	}
	else if(s_SubTab == 4)
	{
		const float LineSize = 20.0f;
		CUIRect Inner;
		DoFeaturePanel(MainView, 345.0f, Localize("FNG laser"), Inner);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngTriggerLaser, Localize("Laser triggerbot?"),
			&g_Config.m_ClZzFngTriggerLaser, &Inner, LineSize);
		Inner.HSplitTop(LineSize, &Button, &Inner);
		Ui()->DoScrollbarOption(&g_Config.m_ClZzFngTriggerDistance,
			&g_Config.m_ClZzFngTriggerDistance, &Button,
			Localize("Trigger attack distance (tiles)"), 1, 26);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngShowDecision, Localize("Laser decision indicator"),
			&g_Config.m_ClZzFngShowDecision, &Inner, LineSize);
		Inner.HSplitTop(5.0f, nullptr, &Inner);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngAutoLaser, Localize("Auto-laser (360 scan)?"),
			&g_Config.m_ClZzFngAutoLaser, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngAimOnFire,
			Localize("Aim on LMB (no automatic fire)"),
			&g_Config.m_ClZzFngAimOnFire, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngAutoLaserBounce, Localize("Allow bounce"),
			&g_Config.m_ClZzFngAutoLaserBounce, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngPingMode, Localize("Пинговый режим"),
			&g_Config.m_ClZzFngPingMode, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngFastMode, Localize("Фаст режим"),
			&g_Config.m_ClZzFngFastMode, &Inner, LineSize);
		DoButton_CheckBoxAutoVMarginAndSet(
			&g_Config.m_ClZzFngDebug, Localize("Debug logging"),
			&g_Config.m_ClZzFngDebug, &Inner, LineSize);
		Inner.HSplitTop(LineSize, &Button, &Inner);
		Ui()->DoScrollbarOption(&g_Config.m_ClZzFngAimMode,
			&g_Config.m_ClZzFngAimMode, &Button,
			Localize("Режим аимбота"), 0, 2);
		Inner.HSplitTop(LineSize, &Button, &Inner);
		Ui()->DoScrollbarOption(&g_Config.m_ClZzFngAutoLaserRaysPerTick,
			&g_Config.m_ClZzFngAutoLaserRaysPerTick, &Button,
			Localize("Rays per tick"), 1, 64);
		Inner.HSplitTop(LineSize, &Button, &Inner);
		Ui()->DoScrollbarOption(&g_Config.m_ClZzFngAutoLaserMaxBounces,
			&g_Config.m_ClZzFngAutoLaserMaxBounces, &Button,
			Localize("Max bounces"), 0, 8);
		Inner.HSplitTop(5.0f, nullptr, &Inner);
		Inner.HSplitTop(LineSize, &Label, &Inner);
		Ui()->DoLabel(&Label, Localize("Режимы: 0 легкий, 1 средний, 2 сильный."),
			14.0f, TEXTALIGN_ML);
	}
	else if(s_SubTab == 5)
	{
		const float LineSize = 20.0f;
		static CScrollRegion s_VisualsScrollRegion;
		vec2 ScrollOffset(0.0f, 0.0f);
		CScrollRegionParams ScrollParams;
		ScrollParams.m_ScrollUnit = 48.0f;
		ScrollParams.m_ScrollbarWidth = 16.0f;
		ScrollParams.m_ScrollbarMargin = 3.0f;
		ScrollParams.m_Flags = CScrollRegionParams::FLAG_CONTENT_STATIC_WIDTH;
		ScrollParams.m_RailBgColor =
			ActiveTheme.m_PanelHover.WithAlpha(0.55f);
		ScrollParams.m_SliderColor = ActiveTheme.m_Accent.WithAlpha(0.62f);
		ScrollParams.m_SliderColorHover = ActiveTheme.m_Accent.WithAlpha(0.82f);
		ScrollParams.m_SliderColorGrabbed = ActiveTheme.m_Accent;
		s_VisualsScrollRegion.Begin(&MainView, &ScrollOffset, &ScrollParams);
		MainView.y += ScrollOffset.y;

		CUIRect Left, Right;
		MainView.VSplitMid(&Left, &Right, 20.0f);
		{
			CUIRect Inner;
			DoFeaturePanel(Left, 498.0f, Localize("Visual effects and animation"),
				Inner);
			Inner.HSplitTop(26.0f, &Button, &Inner);
			static CButtonContainer s_VanillaVisuals;
			if(DoButtonLineSize_Menu(&s_VanillaVisuals,
				   Localize("Use vanilla DDNet visuals"), 0, &Button, 26.0f,
				   false, 0, IGraphics::CORNER_ALL, 13.0f, 0.0f,
				   ActiveTheme.m_Accent.WithAlpha(0.48f)))
				ApplyZZVanillaVisuals();
			Inner.HSplitTop(7.0f, nullptr, &Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualEffects, Localize("Enable visual effects"),
				&g_Config.m_ClZzVisualEffects, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzVisualEffectsIntensity,
				&g_Config.m_ClZzVisualEffectsIntensity, &Button,
				Localize("Effects intensity"), 0, 100);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzClickGuiAnimations, Localize("Click GUI animations"),
				&g_Config.m_ClZzClickGuiAnimations, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzAnimationSpeed,
				&g_Config.m_ClZzAnimationSpeed, &Button,
				Localize("Animation speed"), 25, 200);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzClickGuiBackdropEffects,
				Localize("Click GUI backdrop effects"),
				&g_Config.m_ClZzClickGuiBackdropEffects, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzClickGuiBackdropSoftness,
				&g_Config.m_ClZzClickGuiBackdropSoftness, &Button,
				Localize("Backdrop softness"), 0, 100);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualEffectsMenu, Localize("Animated menu accents"),
				&g_Config.m_ClZzVisualEffectsMenu, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzMenuParticles, Localize("Menu particles"),
				&g_Config.m_ClZzMenuParticles, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzMenuParticleAmount,
				&g_Config.m_ClZzMenuParticleAmount, &Button,
				Localize("Particle amount"), 0, 100);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzCursorTrail, Localize("Cursor trail"),
				&g_Config.m_ClZzCursorTrail, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzCursorTrailLength,
				&g_Config.m_ClZzCursorTrailLength, &Button,
				Localize("Trail length"), 10, 100);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzCursorTrailSize,
				&g_Config.m_ClZzCursorTrailSize, &Button,
				Localize("Trail thickness"), 10, 100);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzCursorTrailParticles,
				&g_Config.m_ClZzCursorTrailParticles, &Button,
				Localize("Trail sparkle"), 0, 100);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzClickBursts, Localize("Click ripple"),
				&g_Config.m_ClZzClickBursts, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzClickBurstStrength,
				&g_Config.m_ClZzClickBurstStrength, &Button,
				Localize("Ripple strength"), 10, 100);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzChatBubbles, Localize("Player chat bubbles"),
				&g_Config.m_ClZzChatBubbles, &Inner, LineSize);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzChatBubbleDuration,
				&g_Config.m_ClZzChatBubbleDuration, &Button,
				Localize("Chat bubble duration"), 2, 10);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzVisualEffectsGameplay,
				Localize("Gameplay screen accents"),
				&g_Config.m_ClZzVisualEffectsGameplay,
				&Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualEffectsFreeze, Localize("Freeze warning pulse"),
				&g_Config.m_ClZzVisualEffectsFreeze, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzVisualEffectsFngPulse,
				Localize("Laser decision pulse"),
				&g_Config.m_ClZzVisualEffectsFngPulse,
				&Inner, LineSize);
		}
		{
			CUIRect Inner;
			DoFeaturePanel(Right, 116.0f, Localize("Theme and aspect ratio"), Inner);
			static CButtonContainer s_aThemeButtons[6];
			for(int ThemeRowIndex = 0; ThemeRowIndex < 2; ++ThemeRowIndex)
			{
				CUIRect ThemeRow;
				Inner.HSplitTop(31.0f, &ThemeRow, &Inner);
				CUIRect Remaining = ThemeRow;
				for(int ThemeColumn = 0; ThemeColumn < 3; ++ThemeColumn)
				{
					const int Theme = ThemeRowIndex * 3 + ThemeColumn;
					CUIRect Cell, Swatch, ThemeButton;
					Remaining.VSplitLeft(ThemeRow.w / 3.0f, &Cell, &Remaining);
					Cell.Margin(2.0f, &Cell);
					Cell.VSplitLeft(18.0f, &Swatch, &ThemeButton);
					const SZZThemePalette ThemePalette = GetZZThemePalette(
						Theme,
						color_cast<ColorRGBA>(ColorHSLA(g_Config.m_UiColor, true)));
					Swatch.Margin(4.0f, &Swatch);
					Swatch.Draw(ThemePalette.m_Accent, IGraphics::CORNER_ALL, 3.0f);
					ThemeButton.VSplitLeft(2.0f, nullptr, &ThemeButton);
					const ColorRGBA ButtonColor = ThemePalette.m_PanelHover.WithAlpha(
						g_Config.m_ClZzTheme == Theme ? 0.92f : 0.50f);
					if(DoButton_Menu(&s_aThemeButtons[Theme], ThemePalette.m_pName,
						   g_Config.m_ClZzTheme == Theme, &ThemeButton,
						   BUTTONFLAG_LEFT, nullptr, IGraphics::CORNER_ALL,
						   4.0f, 0.0f, ButtonColor))
					{
						g_Config.m_ClZzTheme = Theme;
						GameClient()->ApplyZZTheme();
					}
				}
			}
			Inner.HSplitTop(7.0f, nullptr, &Inner);
			CUIRect AspectRow;
			Inner.HSplitTop(31.0f, &AspectRow, &Inner);
			static CButtonContainer s_aAspectButtons[6];
			const char *apAspectNames[] = {"Native", "4:3", "5:4", "16:10", "16:9", "21:9"};
			CUIRect AspectRemaining = AspectRow;
			for(int Aspect = 0; Aspect < 6; ++Aspect)
			{
				CUIRect AspectButton;
				AspectRemaining.VSplitLeft(AspectRow.w / 6.0f, &AspectButton,
					&AspectRemaining);
				AspectButton.Margin(2.0f, &AspectButton);
				const bool Selected = g_Config.m_ClZzAspectRatio == Aspect;
				const ColorRGBA ButtonColor = Selected ?
					ActiveTheme.m_Accent.WithAlpha(0.72f) :
					ActiveTheme.m_PanelHover.WithAlpha(0.50f);
				if(DoButton_Menu(&s_aAspectButtons[Aspect], apAspectNames[Aspect],
					Selected, &AspectButton, BUTTONFLAG_LEFT, nullptr,
					IGraphics::CORNER_ALL, 4.0f, 0.0f, ButtonColor))
					g_Config.m_ClZzAspectRatio = Aspect;
			}
		}
		{
			CUIRect Inner, PresetRow, DetailLeft, DetailRight;
			DoFeaturePanel(Right, 252.0f, Localize("Visual profile"), Inner);
			Inner.HSplitTop(26.0f, &PresetRow, &Inner);
			static CButtonContainer s_aVisualPresetButtons[4];
			const char *apPresetNames[] = {Localize("Clean"), Localize("Balanced"),
				Localize("Vivid"),
				Localize("Performance")};
			CUIRect PresetRemaining = PresetRow;
			for(int Preset = 0; Preset < 4; ++Preset)
			{
				CUIRect PresetButton;
				PresetRemaining.VSplitLeft(PresetRow.w / 4.0f, &PresetButton,
					&PresetRemaining);
				PresetButton.Margin(2.0f, &PresetButton);
				const ColorRGBA PresetColor =
					g_Config.m_ClZzVisualPreset == Preset ? ActiveTheme.m_Accent.WithAlpha(0.72f) : ActiveTheme.m_PanelHover.WithAlpha(0.58f);
				if(DoButton_Menu(&s_aVisualPresetButtons[Preset],
					   apPresetNames[Preset],
					   g_Config.m_ClZzVisualPreset == Preset, &PresetButton,
					   BUTTONFLAG_LEFT, nullptr, IGraphics::CORNER_ALL, 4.0f,
					   0.0f, PresetColor))
					g_Config.m_ClZzVisualPreset = Preset;
			}
			Inner.HSplitTop(8.0f, nullptr, &Inner);
			Inner.VSplitMid(&DetailLeft, &DetailRight, 8.0f);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualAdaptive, Localize("Adaptive effects"),
				&g_Config.m_ClZzVisualAdaptive, &DetailLeft, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzPostProcessing, Localize("Shader bloom"),
				&g_Config.m_ClZzPostProcessing, &DetailLeft, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualHudStats, Localize("Smooth FPS / ping"),
				&g_Config.m_ClZzVisualHudStats, &DetailLeft, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzVisualFeatureToasts,
				Localize("Feature notifications"),
				&g_Config.m_ClZzVisualFeatureToasts,
				&DetailLeft, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualWorldAtmosphere, Localize("Map atmosphere"),
				&g_Config.m_ClZzVisualWorldAtmosphere, &DetailLeft, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualAmbientSweep, Localize("Ambient sweep"),
				&g_Config.m_ClZzVisualAmbientSweep, &DetailLeft, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualPlayerMotion, Localize("Tee motion"),
				&g_Config.m_ClZzVisualPlayerMotion, &DetailRight, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualWeaponEffects, Localize("Weapon effects"),
				&g_Config.m_ClZzVisualWeaponEffects, &DetailRight, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualOtherTrails, Localize("Other player trails"),
				&g_Config.m_ClZzVisualOtherTrails, &DetailRight, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(&g_Config.m_ClZzVisualDynamicLights,
				Localize("Dynamic weapon lights"),
				&g_Config.m_ClZzVisualDynamicLights,
				&DetailRight, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualPlayerPresence, Localize("Player transitions"),
				&g_Config.m_ClZzVisualPlayerPresence, &DetailRight, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVisualFocusBrackets, Localize("Focus brackets"),
				&g_Config.m_ClZzVisualFocusBrackets, &DetailRight, LineSize);
			const float AdvancedY = maximum(DetailLeft.y, DetailRight.y);
			Inner.h = maximum(0.0f, Inner.h - (AdvancedY - Inner.y));
			Inner.y = AdvancedY;
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzPostExposure,
				&g_Config.m_ClZzPostExposure, &Button,
				Localize("HDR exposure"), 70, 140);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzPostBloomSpread,
				&g_Config.m_ClZzPostBloomSpread, &Button,
				Localize("Bloom spread"), 20, 100);
			Inner.HSplitTop(LineSize, &Button, &Inner);
			Ui()->DoScrollbarOption(&g_Config.m_ClZzPostLightDepth,
				&g_Config.m_ClZzPostLightDepth, &Button,
				Localize("World light depth"), 0, 100);
		}
		{
			CUIRect Inner;
			DoFeaturePanel(Right, 116.0f, Localize("Weapon effect colors"), Inner);
			static CButtonContainer s_HammerColor, s_GunColor, s_LaserColor,
				s_GrenadeColor;
			DoLine_ColorPicker(&s_HammerColor, 25.0f, 13.0f, 3.0f, &Inner,
				Localize("Hammer"), &g_Config.m_ClZzWeaponHammerColor,
				ColorRGBA(0.30f, 0.90f, 1.0f, 1.0f), false);
			DoLine_ColorPicker(&s_GunColor, 25.0f, 13.0f, 3.0f, &Inner,
				Localize("Gun"), &g_Config.m_ClZzWeaponGunColor,
				ColorRGBA(1.0f, 0.78f, 0.26f, 1.0f), false);
			DoLine_ColorPicker(&s_LaserColor, 25.0f, 13.0f, 3.0f, &Inner,
				Localize("Laser"), &g_Config.m_ClZzWeaponLaserColor,
				ColorRGBA(0.55f, 0.45f, 1.0f, 1.0f), false);
			DoLine_ColorPicker(&s_GrenadeColor, 25.0f, 13.0f, 3.0f, &Inner,
				Localize("Grenade"),
				&g_Config.m_ClZzWeaponGrenadeColor,
				ColorRGBA(1.0f, 0.34f, 0.28f, 1.0f), false);
		}

		CUIRect ContentEnd = MainView;
		ContentEnd.y = maximum(Left.y, Right.y);
		ContentEnd.h = 0.0f;
		s_VisualsScrollRegion.AddRect(ContentEnd);
		s_VisualsScrollRegion.End();
	}
	else if(s_SubTab == 6)
	{
		const float LineSize = 24.0f;
		CUIRect Left, Right;
		MainView.VSplitMid(&Left, &Right, 20.0f);

		{
			CUIRect Inner, Row;
			DoFeaturePanel(Left, 210.0f, Localize("Recording format"), Inner);

			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Resolution"), 14.0f, TEXTALIGN_ML);
			Inner.HSplitTop(32.0f, &Row, &Inner);
			static CButtonContainer s_aResolutionButtons[3];
			const int aHeights[] = {360, 720, 1080};
			const char *apResolutionNames[] = {"360p", "720p", "1080p"};
			CUIRect Remaining = Row;
			for(int i = 0; i < 3; ++i)
			{
				CUIRect Cell;
				Remaining.VSplitLeft(Row.w / 3.0f, &Cell, &Remaining);
				Cell.Margin(2.0f, &Cell);
				const bool Selected = g_Config.m_ClVideoRecorderHeight == aHeights[i];
				const ColorRGBA ButtonColor = Selected ? ActiveTheme.m_Accent.WithAlpha(0.78f) : ActiveTheme.m_PanelHover.WithAlpha(0.58f);
				if(DoButton_Menu(&s_aResolutionButtons[i], apResolutionNames[i], Selected, &Cell,
					   BUTTONFLAG_LEFT, nullptr, IGraphics::CORNER_ALL, 4.0f, 0.0f, ButtonColor))
					g_Config.m_ClVideoRecorderHeight = aHeights[i];
			}

			Inner.HSplitTop(12.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, Localize("Frame rate"), 14.0f, TEXTALIGN_ML);
			static CButtonContainer s_aFpsButtons[6];
			const int aFpsValues[] = {24, 10, 60, 90, 120, 165};
			for(int RowIndex = 0; RowIndex < 2; ++RowIndex)
			{
				Inner.HSplitTop(32.0f, &Row, &Inner);
				CUIRect RemainingFps = Row;
				for(int Column = 0; Column < 3; ++Column)
				{
					const int Index = RowIndex * 3 + Column;
					CUIRect Cell;
					RemainingFps.VSplitLeft(Row.w / 3.0f, &Cell, &RemainingFps);
					Cell.Margin(2.0f, &Cell);
					char aFpsLabel[16];
					str_format(aFpsLabel, sizeof(aFpsLabel), "%d FPS", aFpsValues[Index]);
					const bool Selected = g_Config.m_ClVideoRecorderFPS == aFpsValues[Index];
					const ColorRGBA ButtonColor = Selected ? ActiveTheme.m_Accent.WithAlpha(0.78f) : ActiveTheme.m_PanelHover.WithAlpha(0.58f);
					if(DoButton_Menu(&s_aFpsButtons[Index], aFpsLabel, Selected, &Cell,
						   BUTTONFLAG_LEFT, nullptr, IGraphics::CORNER_ALL, 4.0f, 0.0f, ButtonColor))
						g_Config.m_ClVideoRecorderFPS = aFpsValues[Index];
				}
			}
		}

		{
			CUIRect Inner;
			DoFeaturePanel(Right, 234.0f, Localize("Video output"), Inner);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClVideoSndEnable, Localize("Record audio"),
				&g_Config.m_ClVideoSndEnable, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClVideoShowhud, Localize("Show HUD"),
				&g_Config.m_ClVideoShowhud, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClVideoShowChat, Localize("Show chat"),
				&g_Config.m_ClVideoShowChat, &Inner, LineSize);
			DoButton_CheckBoxAutoVMarginAndSet(
				&g_Config.m_ClZzVideoFastRender, Localize("Fast video rendering"),
				&g_Config.m_ClZzVideoFastRender, &Inner, LineSize);

			Inner.HSplitTop(14.0f, nullptr, &Inner);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			Ui()->DoLabel(&Label, "MP4 (H.264) / 16:9", 14.0f, TEXTALIGN_ML);
			Inner.HSplitTop(LineSize, &Label, &Inner);
			char aOutputSummary[64];
			str_format(aOutputSummary, sizeof(aOutputSummary), "%d x %d / %d FPS",
				g_Config.m_ClVideoRecorderHeight * 16 / 9,
				g_Config.m_ClVideoRecorderHeight,
				g_Config.m_ClVideoRecorderFPS);
			Ui()->DoLabel(&Label, aOutputSummary, 14.0f, TEXTALIGN_ML);

			Inner.HSplitTop(10.0f, nullptr, &Inner);
			Inner.HSplitTop(32.0f, &Button, &Inner);
			static CButtonContainer s_OpenRecordingsFolderButton;
			if(DoButton_Menu(&s_OpenRecordingsFolderButton, "Open recordings folder", 0, &Button))
			{
				Storage()->CreateFolder("videos", IStorage::TYPE_SAVE);
				char aVideosFolder[IO_MAX_PATH_LENGTH];
				Storage()->GetCompletePath(IStorage::TYPE_SAVE, "videos", aVideosFolder, sizeof(aVideosFolder));
				Client()->ViewFile(aVideosFolder);
			}
		}
	}
	TextRender()->TextColor(OldTextColor);
}
