/**
 * ==================================================
 *   _____ _ _ _             _
 *  |     |_| | |___ ___ ___|_|_ _ _____
 *  | | | | | | | -_|   |   | | | |     |
 *  |_|_|_|_|_|_|___|_|_|_|_|_|___|_|_|_|
 *
 * ==================================================
 *
 * Copyright (c) 2025 Project Millennium
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <imgui.h>
#include <imgui_stdlib.h>
#include <dpi.h>
#include <texture.hh>
#include <animate.h>
#include <i18n.h>
#ifdef _WIN32
#endif
#include <format>
#include <util.h>
#include <renderer.h>

using namespace ImGui;

constexpr const char* discordInviteLink = "https://steambrew.app/discord";
constexpr const char* githubRepositoryUrl = "https://github.com/SteamClientHomebrew/Millennium";

const void RenderBottomNavBar(const char* identifier, float xPos, std::function<void()> buttonRenderCallback, bool setPosManually)
{
    ImGuiIO& io = GetIO();
    ImGuiViewport* viewport = GetMainViewport();
    const float BottomNavBarHeight = ScaleY(115);
    const int FooterContainerWidth = ScaleX(300);

    if (!setPosManually)
        SetCursorPos({ xPos, viewport->Size.y - BottomNavBarHeight + 1 });

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ScaleX(30), ScaleY(30)));
    PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));
    PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.078f, 0.082f, 0.09f, 1.f));

    BeginChild(std::format("##BottomNavBar{}", identifier).c_str(), ImVec2(viewport->Size.x, BottomNavBarHeight), true, ImGuiWindowFlags_NoScrollbar);
    {
        SetCursorPos({ ScaleX(45), GetCursorPosY() + ScaleY(12.5) });
        Image((ImTextureID)(intptr_t)infoIconTexture, ImVec2(ScaleX(25), ScaleY(25)));

        SameLine(0, ScaleX(42));
        const float cursorPosSave = GetCursorPosX();

        SetCursorPosY(GetCursorPosY() - ScaleX(12));
        TextColored(ImVec4(0.322f, 0.325f, 0.341f, 1.0f), "%s", Locale::Get("installerDisclaimer1"));

        SetCursorPos({ cursorPosSave, GetCursorPosY() - ScaleY(20) });
        TextColored(ImVec4(0.322f, 0.325f, 0.341f, 1.0f), "%s", Locale::Get("installerDisclaimer2"));

        SameLine(0);
        SetCursorPosY(GetCursorPosY() - ScaleY(25));

        const float buttonPos = GetCursorPosY();

        SetCursorPosX(xPos + GetCursorPosX() + GetContentRegionAvail().x - FooterContainerWidth - ScaleX(55));
        SetCursorPosY(GetCursorPosY() + ScaleY(10));

        // ── Language button ────────────────────────────────────────────────
        Image((ImTextureID)(intptr_t)languageIconTexture, ImVec2(ScaleX(30), ScaleY(30)));

        static bool isLangButtonHovered = false;
        float langTooltipAlpha = EaseInOutFloat(std::format("##LangTooltipAlpha{}", identifier).c_str(), 0.f, 1.f, isLangButtonHovered, 0.3f);

        if (langTooltipAlpha != 0.f) {
            SetMouseCursor(ImGuiMouseCursor_Hand);
            PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            PushStyleColor(ImGuiCol_Border, ImVec4(0.18f, 0.184f, 0.192f, 1.0f));
            PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ScaleX(10), ScaleY(10)));
            PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
            PushStyleVar(ImGuiStyleVar_Alpha, langTooltipAlpha);
            PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.098f, 0.102f, 0.11f, 1.0f));
            SetTooltip("%s", Locale::Get("tooltipLanguage"));
            PopStyleVar(4);
            PopStyleColor(3);
        }

        if (IsItemClicked())
            OpenPopup("##LangPopup");

        isLangButtonHovered = IsItemHovered() || (IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && IsMouseDown(ImGuiMouseButton_Left));

        // ── Language popup ─────────────────────────────────────────────────
        {
            ImGuiIO& io = GetIO();
            ImFont* vietItemFont = (io.Fonts->Fonts.Size > 2) ? io.Fonts->Fonts[2] : nullptr;
            ImFont* dropdownFont = (io.Fonts->Fonts.Size > 3) ? io.Fonts->Fonts[3] : nullptr;

            const auto& langs = Locale::GetAvailableLanguages();
            const std::string& currentLangId = Locale::GetCurrentLanguageId();

            const float popupWidth = ScaleX(400);
            const float popupHeight = ScaleY(500);
            float anim = EaseInOutFloat("##LangPopupAnim", 0.f, 1.f, IsPopupOpen("##LangPopup"), 0.35f);

            const float popupBtnY = viewport->Size.y - BottomNavBarHeight;
            float popupY = popupBtnY - popupHeight + ScaleY(6) * (1.f - anim);
            SetNextWindowPos({ viewport->Size.x - popupWidth - ScaleX(50), popupY });
            SetNextWindowSize({ popupWidth, popupHeight });
            SetNextWindowBgAlpha(anim);

            PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.098f, 0.102f, 0.11f, 1.0f));
            PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.155f, 0.165f, 1.0f));
            PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.22f, 0.225f, 0.235f, 1.0f));
            PushStyleColor(ImGuiCol_Border, ImVec4(0.18f, 0.184f, 0.192f, 1.0f));
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ScaleX(10), ScaleY(10)));
            PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ScaleX(8), ScaleY(6)));
            PushStyleVar(ImGuiStyleVar_PopupRounding, ScaleX(6));

            if (dropdownFont)
                PushFont(dropdownFont);

            if (BeginPopup("##LangPopup", ImGuiWindowFlags_NoMove)) {
                if (!::IsWindowFocused())
                    CloseCurrentPopup();
                PushStyleVar(ImGuiStyleVar_Alpha, anim);
                for (const auto& lang : langs) {
                    bool isSelected = (lang.id == currentLangId);
                    bool useVietFont = (lang.id == "vietnamese") && (vietItemFont != nullptr);
                    if (useVietFont) {
                        if (dropdownFont)
                            PopFont();
                        PushFont(vietItemFont);
                    }
                    if (Selectable(lang.displayName.c_str(), isSelected, 0, ImVec2(popupWidth, 0))) {
                        Locale::SetLanguage(lang.id);
                        RequestFontRebuild();
                        CloseCurrentPopup();
                    }
                    if (isSelected)
                        SetItemDefaultFocus();
                    if (useVietFont) {
                        PopFont();
                        if (dropdownFont)
                            PushFont(dropdownFont);
                    }
                }
                PopStyleVar();
                EndPopup();
            }

            if (dropdownFont)
                PopFont();

            PopStyleVar(3);
            PopStyleColor(5);
        }

        SameLine(0, ScaleX(25));
        SetCursorPosY(GetCursorPosY() - ScaleY(15));

        Image((ImTextureID)(intptr_t)discordIconTexture, ImVec2(ScaleX(30), ScaleY(30)));

        static bool isDiscordButtonHovered = false;
        float discordIconHoverTransparency = EaseInOutFloat(std::format("##DiscordIconHover{}", identifier).c_str(), 0.f, 1.f, isDiscordButtonHovered, 0.3f);

        /** Check if the animation has started */
        if (discordIconHoverTransparency != 0.f) {
            SetMouseCursor(ImGuiMouseCursor_Hand);
            PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            PushStyleColor(ImGuiCol_Border, ImVec4(0.18f, 0.184f, 0.192f, 1.0f));
            PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ScaleX(10), ScaleY(10)));
            PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
            PushStyleVar(ImGuiStyleVar_Alpha, discordIconHoverTransparency);
            PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.098f, 0.102f, 0.11f, 1.0f));
            SetTooltip("%s", Locale::Get("tooltipDiscord"));

            if (IsItemClicked()) {
                OpenUrl(discordInviteLink);
            }

            PopStyleVar(4);
            PopStyleColor(3);
        }

        isDiscordButtonHovered = IsItemHovered() || (IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && IsMouseDown(ImGuiMouseButton_Left));

        SameLine(0, ScaleX(25));
        SetCursorPosY(GetCursorPosY() - ScaleY(15));

        Image((ImTextureID)(intptr_t)gtihubIconTexture, ImVec2(ScaleX(30), ScaleY(30)));

        static bool isGithubButtonHovered = false;
        float githubIconHoverTransparency = EaseInOutFloat(std::format("##GithubIconHover{}", identifier).c_str(), 0.f, 1.f, isGithubButtonHovered, 0.3f);

        /** Check if the animation has started */
        if (githubIconHoverTransparency != 0.f) {
            SetMouseCursor(ImGuiMouseCursor_Hand);
            PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            PushStyleColor(ImGuiCol_Border, ImVec4(0.18f, 0.184f, 0.192f, 1.0f));
            PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ScaleX(10), ScaleY(10)));
            PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
            PushStyleVar(ImGuiStyleVar_Alpha, githubIconHoverTransparency);
            PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.098f, 0.102f, 0.11f, 1.0f));
            SetTooltip("%s", Locale::Get("tooltipGithub"));

            if (IsItemClicked()) {
                OpenUrl(githubRepositoryUrl);
            }

            PopStyleVar(4);
            PopStyleColor(3);
        }

        isGithubButtonHovered = IsItemHovered() || (IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && IsMouseDown(ImGuiMouseButton_Left));

        SameLine(0, ScaleX(25));
        SetCursorPosY(buttonPos);

        PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        {
            buttonRenderCallback();
        }
        PopStyleColor(2);
    }
    EndChild();

    PopStyleVar(2);
    PopStyleColor(2);
}