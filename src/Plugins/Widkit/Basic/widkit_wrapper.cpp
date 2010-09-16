
/******************************************************************************
* MODULE     : widkit_wrapper.hpp
* DESCRIPTION: Conversions between widget and wk_widget
* COPYRIGHT  : (C) 2007  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "promise.hpp"
#include "url.hpp"
#include "Widkit/wk_widget.hpp"
#include "message.hpp"
#include "window.hpp"
#include "dictionary.hpp"

#define THIS wk_widget (this)

static void noop () {}
widget box_widget (scheme_tree p, string s, color col, bool trans, bool ink);

/******************************************************************************
* Type conversions
******************************************************************************/

array<widget>
abstract (array<wk_widget> a) {
  int i, n= N(a);
  array<widget> b (n);
  for (i=0; i<n; i++) b[i]= abstract (a[i]);
  return b;
}

array<wk_widget>
concrete (array<widget> a) {
  int i, n= N(a);
  array<wk_widget> b (n);
  for (i=0; i<n; i++) b[i]= concrete (a[i]);
  return b;
}

class abstract_promise_rep: public promise_rep<widget> {
  promise<wk_widget> p;
public:
  abstract_promise_rep (promise<wk_widget> p2): p (p2) {}
  tm_ostream& print (tm_ostream& out) { return out << p; }
  widget eval () { return abstract (p ()); }
};

promise<widget>
abstract (promise<wk_widget> pw) {
  return tm_new<abstract_promise_rep> (pw);
}

class concrete_promise_rep: public promise_rep<wk_widget> {
  promise<widget> p;
public:
  concrete_promise_rep (promise<widget> p2): p (p2) {}
  tm_ostream& print (tm_ostream& out) { return out << p; }
  wk_widget eval () { return concrete (p ()); }
};

promise<wk_widget>
concrete (promise<widget> pw) {
  return tm_new<concrete_promise_rep> (pw);
}

/******************************************************************************
* Exported special widgets
******************************************************************************/

widget
horizontal_list (array<widget> a) {
  return abstract (horizontal_list (concrete (a)));
}

widget
vertical_list (array<widget> a) {
  return abstract (vertical_list (concrete (a)));
}

widget
horizontal_menu (array<widget> a) {
  return abstract (horizontal_array (concrete (a), -1));
}

widget
vertical_menu (array<widget> a) {
  return abstract (vertical_menu (concrete (a)));
}

widget
tile_menu (array<widget> a, int cols) {
  return abstract (tile (concrete (a), cols));
}

widget
switch_widget (array<widget> a, array<string> name, int init) {
  return abstract (switch_widget (concrete (a), name, init));
}

widget
optional_widget (widget w, bool on) {
  return abstract (optional_widget (concrete (w), on));
}

widget
empty_widget () {
  return abstract (glue_wk_widget (false, false, 0, 0));
}

widget
glue_widget (bool hx, bool vx, SI w, SI h) {
  return abstract (glue_wk_widget (hx, vx, w, h));
}

widget
menu_separator (bool vert) {
  return abstract (separator_wk_widget (2*PIXEL, 2*PIXEL, vert));
}

widget
text_widget (string s, bool tsp, string lan) {
  return abstract (text_wk_widget (s, tsp, lan));
}

widget
menu_text_widget (string s, color col, bool tsp, string lan, bool tt) {
  return abstract (menu_text_wk_widget (s, col, tsp, lan, tt));
}

widget
text_widget (string s, color col, bool tsp, string lan) {
  return menu_text_widget (s, col, tsp, lan, false);
}

widget
xpm_widget (url file_name) {
  return abstract (xpm_wk_widget (file_name, true));
}

widget
command_button (widget w, command cmd, bool button_flag) {
  return abstract (command_button (concrete (w), cmd, button_flag));
}

widget
command_button (widget lw, widget cw, widget rw, command cmd, bool e, bool c) {
  return abstract (command_button (concrete (lw), concrete (cw),
				   concrete (rw), cmd, e, c));
}

widget
menu_group (string name, string lan) {
  widget lw= empty_widget ();
  widget cw= text_widget (name, dark_grey, false, lan);
  widget rw= empty_widget ();
  return command_button (lw, cw, rw, noop, false, true);
}

widget
menu_button (widget w, command cmd, string pre, string ks, bool ok) {
  if (pre == "" && ks == "") return command_button (w, cmd, false);
  else {
    color  c = ok? black: dark_grey;
    widget lw= empty_widget ();
    widget rw= menu_text_widget (ks, c, true, "english", true);
    if (pre != "") {
      string s= "";
      if (pre == "v") s= "<checked>";
      if (pre == "o") s= "<circ>";
      if (pre == "*") s= "<bullet>";
      if (s != "") lw= box_widget (tree (TUPLE), s, c, true, false);
    }
    return command_button (lw, w, rw, cmd, ok, false);
  }
}

widget
pulldown_button (widget w, promise<widget> pw) {
  return abstract (pulldown_button (concrete (w), concrete (pw)));
}

widget
pullright_button (widget w, promise<widget> pw) {
  return abstract (pullright_button (concrete (w), concrete (pw)));
}

widget
popup_widget (widget w) {
  return abstract (popup_widget (concrete (w), center));
}

widget
canvas_widget (widget w) {
  return abstract (canvas_widget (concrete (w), north_west));
}

widget
input_text_widget (command call_back, string type, array<string> def) {
  return abstract (input_text_wk_widget (call_back, type, def));
}

widget
inputs_list_widget (command call_back, array<string> prompts) {
  return abstract (inputs_list_wk_widget (call_back, prompts));
}

widget
file_chooser_widget (command cmd, string type, string mgn) {
  return abstract (file_chooser_wk_widget (cmd, type, mgn));
}

widget
balloon_widget (widget w, widget help) {
  return abstract (balloon_widget (concrete (w), concrete (help)));
}

widget
wait_widget (SI w, SI h, string message) {
  return abstract (wait_wk_widget (w, h, message));
}

widget
texmacs_widget (int mask, command quit) {
  return abstract (texmacs_wk_widget (mask, quit));
}

widget
plain_window_widget (widget wid, string s) {
  return abstract (plain_window_widget (concrete (wid), s));
}

widget
popup_window_widget (widget wid, string s) {
  return abstract (popup_window_widget (concrete (wid), s));
}

void
destroy_window_widget (widget w) {
  destroy_window_widget (concrete (w));
}

/******************************************************************************
* Type checking
******************************************************************************/

void
check_type_void (blackbox bb, string s) {
  if (!is_nil (bb)) {
    cerr << "\nslot type= " << s << "\n";
    FAILED ("type mismatch");
  }
}

template<class T> void
check_type (blackbox bb, string s) {
  if (type_box (bb) != type_helper<T>::id) {
    cerr << "\nslot type= " << s << "\n";
    FAILED ("type mismatch");
  }
}

template<class T1, class T2> inline void
check_type (blackbox bb, string s) {
  check_type<pair<T1,T2> > (bb, s);
}

/******************************************************************************
* Widget geometry
******************************************************************************/

SI get_dx (gravity grav, SI w);
SI get_dy (gravity grav, SI h);

void
principal_widget_check (wk_widget wid) {
  // FIXME: Positions should really be computed relative to parent widgets.
  // Currently, we only allow geometry access of the unique child of
  // a window widget.
  if (wid->win != NULL && wid != concrete (wid->win->get_widget ()) [0]) {
    cerr << "Widget= " << wid << "\n";
    FAILED ("invalid geometry access");
  }
}

void
set_geometry (wk_widget wid, SI x, SI y, SI w, SI h) {
  if (wid->is_window_widget ()) {
    wid->win->set_position (x, y);
    wid->win->set_size (w, h);
  }
  else {
    principal_widget_check (wid); // FIXME: we should use parent's coordinates
    wid << emit_position (x, y, w, h, north_west);
  }
}

void
get_geometry (wk_widget wid, SI& x, SI& y, SI& w, SI& h) {
  if (wid->is_window_widget ()) {
    wid->win->get_position (x, y);
    wid->win->get_size (w, h);
  }
  else {
    principal_widget_check (wid); // FIXME: we should use parent's coordinates
    x= wid->ox - get_dx (wid->grav, wid->w);
    y= wid->oy - get_dy (wid->grav, wid->h);
    w= wid->w;
    h= wid->h;
  }
}

/******************************************************************************
* Sending messages
******************************************************************************/

void
send_bool (wk_widget w, string key, blackbox val) {
  ASSERT (type_box (val) == type_helper<bool>::id, "type mismatch");
  w << set_string (key, open_box<bool> (val)? string ("on"): string ("off"));
}

void
send_int (wk_widget w, string key, blackbox val) {
  ASSERT (type_box (val) == type_helper<int>::id, "type mismatch");
  w << set_integer (key, open_box<int> (val));
}

void
send_string (wk_widget w, string key, blackbox val) {
  ASSERT (type_box (val) == type_helper<string>::id, "type mismatch");
  w << set_string (key, open_box<string> (val));
}

void
send_coord2 (wk_widget w, string key, blackbox val) {
  typedef pair<SI,SI> coord2;
  ASSERT (type_box (val) == type_helper<coord2>::id, "type mismatch");
  coord2 p= open_box<coord2> (val);
  w << set_coord2 (key, p.x1, p.x2);
}

void
send_coord4 (wk_widget w, string key, blackbox val) {
  typedef quartet<SI,SI,SI,SI> coord4;
  ASSERT (type_box (val) == type_helper<coord4>::id, "type mismatch");
  coord4 p= open_box<coord4> (val);
  w << set_coord4 (key, p.x1, p.x2, p.x3, p.x4);
}

void
send_position (wk_widget w, blackbox val) {
  typedef pair<SI,SI> coord2;
  ASSERT (type_box (val) == type_helper<coord2>::id, "type mismatch");
  coord2 p= open_box<coord2> (val);
  if (w->is_window_widget ()) w->win->set_position (p.x1, p.x2);
  else {
    SI x, y, W, H;
    get_geometry (w, x, y, W, H);
    set_geometry (w, p.x1, p.x2, W, H);
  }
}

void
send_size (wk_widget w, blackbox val) {
  typedef pair<SI,SI> coord2;
  ASSERT (type_box (val) == type_helper<coord2>::id, "type mismatch");
  coord2 p= open_box<coord2> (val);
  if (w->is_window_widget ()) w->win->set_size (p.x1, p.x2);
  else {
    SI x, y, W, H;
    get_geometry (w, x, y, W, H);
    set_geometry (w, x, y, p.x1, p.x2);
  }
}

void
send_update (wk_widget w, blackbox val) {
  ASSERT (is_nil (val), "type mismatch");
  w << emit_update ();
}

void
send_keyboard (wk_widget w, blackbox val) {
  typedef pair<string,time_t> keypress;
  ASSERT (type_box (val) == type_helper<keypress>::id, "type mismatch");
  keypress k= open_box<keypress> (val);
  w << emit_keypress (k.x1, k.x2);
}

void
send_keyboard_focus (wk_widget w, blackbox val) {
  ASSERT (type_box (val) == type_helper<bool>::id, "type mismatch");
  w->win->set_keyboard_focus (abstract (w), open_box<bool> (val));
}

void
send_mouse (wk_widget w, blackbox val) {
  typedef quintuple<string,SI,SI,int,time_t> mouse;
  ASSERT (type_box (val) == type_helper<mouse>::id, "type mismatch");
  mouse m= open_box<mouse> (val);
  // FIXME: we should assume the position in the local coordinates
  w << emit_mouse (m.x1, m.x2, m.x3, m.x4, m.x5);
}

void
send_mouse_grab (wk_widget w, blackbox val) {
  ASSERT (type_box (val) == type_helper<bool>::id, "type mismatch");
  w->win->set_mouse_grab (abstract (w), open_box<bool> (val));
}

void
send_mouse_pointer (wk_widget w, blackbox val) {
  typedef pair<string,string> change_pointer;
  ASSERT (type_box (val) == type_helper<change_pointer>::id, "type mismatch");
  change_pointer cp= open_box<change_pointer> (val);
  w->win->set_mouse_pointer (abstract (w), cp.x1, cp.x2);
}

void
send_invalidate (wk_widget w, blackbox val) {
  typedef quartet<SI,SI,SI,SI> coord4;
  ASSERT (type_box (val) == type_helper<coord4>::id, "type mismatch");
  coord4 p= open_box<coord4> (val);
  w << emit_invalidate (w->ox + p.x1, w->oy + p.x2,
			w->ox + p.x3, w->oy + p.x4);
}

void
send_invalidate_all (wk_widget w, blackbox val) {
  ASSERT (is_nil (val), "type mismatch");
  w << emit_invalidate_all ();
}

void
send_repaint (wk_widget w, blackbox val) {
  typedef quartet<SI,SI,SI,SI> repaint;
  ASSERT (type_box (val) == type_helper<repaint>::id, "type mismatch");
  repaint r= open_box<repaint> (val);
  bool stop_flag= false;
  // FIXME: we should assume local coordinates for repainting
  w << emit_repaint (r.x1, r.x2, r.x3, r.x4, stop_flag);
}

void
send_delayed_message (wk_widget w, blackbox val) {
  typedef pair<string,time_t> delayed;
  ASSERT (type_box (val) == type_helper<delayed>::id, "type mismatch");
  delayed dm= open_box<delayed> (val);
  w << emit_alarm (dm.x1, dm.x2);
}

void
send_destroy (wk_widget w, blackbox val) {
  ASSERT (is_nil (val), "type mismatch");
  w << emit_destroy ();
}

void
wk_widget_rep::send (slot s, blackbox val) {
  switch (s) {
  case SLOT_IDENTIFIER:
    check_type<int> (val, "SLOT_IDENTIFIER");
    THIS << emit_attach_window (get_window (open_box<int> (val)));
    break;
  case SLOT_VISIBILITY:
    check_type<bool> (val, "SLOT_VISIBILITY");
    win->set_visibility (open_box<bool> (val));
    break;
  case SLOT_FULL_SCREEN:
    check_type<bool> (val, "SLOT_FULL_SCREEN");
    win->set_full_screen (open_box<bool> (val));
    break;
  case SLOT_NAME:
    send_string (THIS, "window name", val);
    break;
  case SLOT_SIZE:
    send_size (THIS, val);
    break;
  case SLOT_POSITION:
    send_position (THIS, val);
    break;
  case SLOT_UPDATE:
    send_update (THIS, val);
    break;
  case SLOT_KEYBOARD:
    send_keyboard (THIS, val);
    break;
  case SLOT_KEYBOARD_FOCUS:
    send_keyboard_focus (THIS, val);
    break;
  case SLOT_MOUSE:
    send_mouse (THIS, val);
    break;
  case SLOT_MOUSE_GRAB:
    send_mouse_grab (THIS, val);
    break;
  case SLOT_MOUSE_POINTER:
    send_mouse_pointer (THIS, val);
    break;
  case SLOT_INVALIDATE:
    send_invalidate (THIS, val);
    break;
  case SLOT_INVALIDATE_ALL:
    send_invalidate_all (THIS, val);
    break;
  case SLOT_REPAINT:
    send_repaint (THIS, val);
    break;
  case SLOT_DELAYED_MESSAGE:
    send_delayed_message (THIS, val);
    break;
  case SLOT_DESTROY:
    send_destroy (THIS, val);
    break;
  case SLOT_CURSOR:
   // send_coord2 (THIS, "cursor", val); 
    // this message is currently ignored. Used only in TeXmacs/Qt
    break;
      
  case SLOT_SHRINKING_FACTOR:
    send_int (THIS, "shrinking factor", val);
    break;
  case SLOT_EXTENTS:
    send_coord4 (THIS, "extents", val);
    break;
  case SLOT_SCROLLBARS_VISIBILITY:
    send_int (THIS, "scrollbars", val);
    break;
  case SLOT_SCROLL_POSITION:
    send_coord2 (THIS, "scroll position", val);
    break;

  case SLOT_HEADER_VISIBILITY:
    send_bool (THIS, "header", val);
    break;
  case SLOT_MAIN_ICONS_VISIBILITY:
    send_bool (THIS, "main icons", val);
    break;
  case SLOT_CONTEXT_ICONS_VISIBILITY:
    send_bool (THIS, "context icons", val);
    break;
  case SLOT_USER_ICONS_VISIBILITY:
    send_bool (THIS, "user icons", val);
    break;
  case SLOT_FOOTER_VISIBILITY:
    send_bool (THIS, "footer flag", val);
    break;
  case SLOT_LEFT_FOOTER:
    send_string (THIS, "left footer", val);
    break;
  case SLOT_RIGHT_FOOTER:
    send_string (THIS, "right footer", val);
    break;
  case SLOT_INTERACTIVE_MODE:
    send_bool (THIS, "interactive mode", val);
    break;

  case SLOT_STRING_INPUT:
    send_string (THIS, "input", val);
    break;
  case SLOT_INPUT_TYPE:
    send_string (THIS, "type", val);
    break;
  case SLOT_INPUT_PROPOSAL:
    send_string (THIS, "default", val);
    break;
  case SLOT_FILE:
    send_string (THIS, "file", val);
    break;
  case SLOT_DIRECTORY:
    send_string (THIS, "directory", val);
    break;
  default:
    FAILED ("cannot handle slot type");
  }
}

/******************************************************************************
* Querying
******************************************************************************/

blackbox
query_bool (wk_widget w, string key, int type_id) {
  ASSERT (type_id == type_helper<bool>::id, "type mismatch");
  string s;
  w << get_string (key, s);
  return close_box<bool> (s == "on");
}

blackbox
query_int (wk_widget w, string key, int type_id) {
  ASSERT (type_id == type_helper<int>::id, "type mismatch");
  int i;
  w << get_integer (key, i);
  return close_box<int> (i);
}

blackbox
query_string (wk_widget w, string key, int type_id) {
  ASSERT (type_id == type_helper<string>::id, "type mismatch");
  string s;
  w << get_string (key, s);
  return close_box<string> (s);
}

blackbox
query_coord2 (wk_widget w, string key, int type_id) {
  typedef pair<SI,SI> coord2;
  ASSERT (type_id == type_helper<coord2>::id, "type mismatch");
  SI c1, c2;
  w << get_coord2 (key, c1, c2);
  return close_box<coord2> (coord2 (c1, c2));
}

blackbox
query_coord4 (wk_widget w, string key, int type_id) {
  typedef quartet<SI,SI,SI,SI> coord4;
  ASSERT (type_id == type_helper<coord4>::id, "type mismatch");
  SI c1, c2, c3, c4;
  w << get_coord4 (key, c1, c2, c3, c4);
  return close_box<coord4> (coord4 (c1, c2, c3, c4));
}

blackbox
query_size (wk_widget w, int type_id) {
  typedef pair<SI,SI> coord2;
  ASSERT (type_id == type_helper<coord2>::id, "type mismatch");
  SI x, y, W, H;
  get_geometry (w, x, y, W, H);
  return close_box<coord2> (coord2 (W, H));
}

blackbox
query_position (wk_widget w, int type_id) {
  typedef pair<SI,SI> coord2;
  ASSERT (type_id == type_helper<coord2>::id, "type mismatch");
  SI x, y, W, H;
  get_geometry (w, x, y, W, H);
  return close_box<coord2> (coord2 (x, y));
}

blackbox
query_keyboard_focus (wk_widget w, int type_id) {
  ASSERT (type_id == type_helper<bool>::id, "type mismatch");
  return close_box<bool> (w->win->get_keyboard_focus (abstract (w)));
}

blackbox
query_mouse_grab (wk_widget w, int type_id) {
  ASSERT (type_id == type_helper<bool>::id, "type mismatch");
  return close_box<bool> (w->win->get_mouse_grab (abstract (w)));
}

blackbox
wk_widget_rep::query (slot s, int type_id) {
  switch (s) {
  case SLOT_IDENTIFIER:
    ASSERT (type_id == type_helper<int>::id,
	    "int expected (SLOT_IDENTIFIER)");
    return close_box<int> (get_identifier (win));
  case SLOT_RENDERER:
    ASSERT (type_id == type_helper<renderer>::id,
	    "renderer expected (SLOT_RENDERER)");
    return close_box<renderer> (win->get_renderer ());
  case SLOT_SIZE:
    return query_size (THIS, type_id);
  case SLOT_POSITION:
    return query_position (THIS, type_id);
  case SLOT_KEYBOARD_FOCUS:
    return query_keyboard_focus (THIS, type_id);
  case SLOT_MOUSE_GRAB:
    return query_mouse_grab (THIS, type_id);

  case SLOT_EXTENTS:
    return query_coord4 (THIS, "extents", type_id);
  case SLOT_VISIBLE_PART:
    return query_coord4 (THIS, "visible", type_id);
  case SLOT_SCROLLBARS_VISIBILITY:
    return query_int (THIS, "scrollbars", type_id);
  case SLOT_SCROLL_POSITION:
    return query_coord2 (THIS, "scroll position", type_id);

  case SLOT_HEADER_VISIBILITY:
    return query_bool (THIS, "header", type_id);
  case SLOT_MAIN_ICONS_VISIBILITY:
    return query_bool (THIS, "main icons", type_id);
  case SLOT_CONTEXT_ICONS_VISIBILITY:
    return query_bool (THIS, "context icons", type_id);
  case SLOT_USER_ICONS_VISIBILITY:
    return query_bool (THIS, "user icons", type_id);
  case SLOT_FOOTER_VISIBILITY:
    return query_bool (THIS, "footer flag", type_id);
  case SLOT_INTERACTIVE_MODE:
    return query_bool (THIS, "interactive mode", type_id);
  case SLOT_INTERACTIVE_INPUT:
    return query_string (THIS, "interactive input", type_id);

  case SLOT_STRING_INPUT:
    return query_string (THIS, "input", type_id);
  default:
    FAILED ("cannot handle slot type");
    return blackbox ();
  }
}

/******************************************************************************
* Notification of state changes
******************************************************************************/

void
notify_keyboard_focus (wk_widget w, blackbox val) {
  ASSERT (type_box (val) == type_helper<bool>::id, "type mismatch");
  w << emit_keyboard_focus (open_box<bool> (val));
}

void
notify_mouse_grab (wk_widget w, blackbox val) {
  ASSERT (type_box (val) == type_helper<bool>::id, "type mismatch");
  w << emit_mouse_grab (open_box<bool> (val));
}

void
wk_widget_rep::notify (slot s, blackbox new_val) {
  switch (s) {
  case SLOT_SIZE:
    check_type<SI,SI> (new_val, "SLOT_SIZE");
    THIS << emit_resize ();
    if (is_window_widget ())
      send_size (THIS [0], new_val);
    break;
  case SLOT_POSITION:
    check_type<SI,SI> (new_val, "SLOT_POSITION");
    THIS << emit_move ();
    break;
  case SLOT_KEYBOARD_FOCUS:
    notify_keyboard_focus (THIS, new_val);
    break;
  case SLOT_MOUSE_GRAB:
    notify_mouse_grab (THIS, new_val);
    break;
  default:
    break;
  }
  widget_rep::notify (s, new_val);
}

/******************************************************************************
* Read and write access of subwidgets
******************************************************************************/

widget
wk_widget_rep::read (slot s, blackbox index) {
  switch (s) {
  case SLOT_WINDOW:
    check_type_void (index, "SLOT_WINDOW");
    return win -> get_widget ();
  case SLOT_FORM_FIELD:
    check_type<int> (index, "SLOT_FORM_FIELD");
    return abstract (THIS [0] ["inputs"] [open_box<int> (index)] ["input"]);
  case SLOT_FILE:
    check_type_void (index, "SLOT_FILE");
    return abstract (THIS [0] ["file"] ["input"]);
  case SLOT_DIRECTORY:
    check_type_void (index, "SLOT_DIRECTORY");
    return abstract (THIS [0] ["directory"] ["input"]);
  default:
    FAILED ("cannot handle slot type");
    return widget ();
  }
}

void
wk_widget_rep::write (slot s, blackbox index, widget w) {
  switch (s) {
  case SLOT_MAIN_MENU:
    check_type_void (index, "SLOT_MAIN_MENU");
    THIS << set_widget ("menu bar", concrete (w));
    break;
  case SLOT_MAIN_ICONS:
    check_type_void (index, "SLOT_MAIN_ICONS");
    THIS << set_widget ("main icons bar", concrete (w));
    break;
  case SLOT_CONTEXT_ICONS:
    check_type_void (index, "SLOT_CONTEXT_ICONS");
    THIS << set_widget ("context icons bar", concrete (w));
    break;
  case SLOT_USER_ICONS:
    check_type_void (index, "SLOT_USER_ICONS");
    THIS << set_widget ("user icons bar", concrete (w));
    break;
  case SLOT_CANVAS:
    check_type_void (index, "SLOT_CANVAS");
    THIS << set_widget ("scrollable", concrete (w));
    break;
  case SLOT_INTERACTIVE_PROMPT:
    check_type_void (index, "SLOT_INTERACTIVE_PROMPT");
    THIS << set_widget ("interactive prompt", concrete (w));
    break;
  case SLOT_INTERACTIVE_INPUT:
    check_type_void (index, "SLOT_INTERACTIVE_INPUT");
    THIS << set_widget ("interactive input", concrete (w));
    break;
  default:
    FAILED ("cannot handle slot type");
  }
}
