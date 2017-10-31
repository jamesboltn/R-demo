#pragma once

namespace GuruLib
{
	typedef unsigned int MessageID;
	typedef long ResourceID;

	class ControlMessageHandler;
	typedef long(ControlMessageHandler::*CtrlMessageHandler)(WPARAM wParam, LPARAM lParam);
	typedef std::pair<MessageID, ResourceID> CommandMessageID, NotifyMessageID;
	typedef std::unordered_map<std::pair<MessageID, ResourceID>, std::function<long(WPARAM wParam, LPARAM lParam)> > CommandMessageMap, NotifyMessageMap;

	class Window;
	typedef long(Window::*MessageHandler)(WPARAM wParam, LPARAM lParam);

	typedef enum
	{
		VERTICAL,
		HORIZONTAL
	}ScrollBarStyle, Dimension;

	typedef enum
	{
		PIXEL,
		DIALOG_UNIT
	}UnitType;

	typedef enum
	{
		LEFT_SIDE,
		RIGHT_SIDE,
		CENTER,
		TOP_LEFT,
		TOP_RIGHT,
		TOP_CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		BOTTOM_CENTER,
		TOP_SIDE,
		BOTTOM_SIDE
	}AlignOrientation;
}

namespace std {
	template <>
	struct hash<std::pair<GuruLib::MessageID, GuruLib::ResourceID>>
	{
		std::size_t operator()(const std::pair<GuruLib::MessageID, GuruLib::ResourceID>& id) const
		{
			std::size_t hash1 = std::hash<GuruLib::MessageID>()(id.first);
			std::size_t hash2 = std::hash<GuruLib::ResourceID>()(id.second);
			return (hash1 ^ (hash2 << 1)) >> 1;
		}
	};
}

#define HOST_WINDOW reinterpret_cast<Window*>(::GetWindowLongPtr(UIBase::GetLastCreatedWindow(), GWLP_USERDATA))

#define AddCtrlWindowHandler(MsgID, Control, Handler) HOST_WINDOW->_AddControlHandler(HOST_WINDOW, MsgID, Control, Handler)
#define AddCtrlPageHandler(MsgID, Control, Handler, Page) HOST_WINDOW->_AddControlHandler(Page, MsgID, Control, Handler)

#define AddWindowHandler(MsgID, Handler) HOST_WINDOW->_AddHandler(HOST_WINDOW, MsgID, Handler)
#define AddPageHandler(MsgID, Handler, Page) HOST_WINDOW->_AddHandler(Page, MsgID, Handler)

#define MN_CLICKED 0
#define WM_DESTROYING WM_DESTROY

#define Y VERTICAL
#define X HORIZONTAL


