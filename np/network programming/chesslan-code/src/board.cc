/*
 * $Revision: 70 $ * tough task 
 */
#define _DEBUG true

#include <sigc++/compatibility.h>
#include "config.h"
#include "board.hh"
#include "makros.hh"

#define SQUARE_SIZE 45

board::board ():
m_canvas (NULL)
{
}
board::board (int n_x, int n_y):
m_canvas (NULL)
{
        // alignment for widgets

        Gtk::Alignment * alignment
                = manage (new Gtk::Alignment (0.5, 0.5, 0.0, 0.0));
        Gtk::VBox::pack_start (*alignment);

        // frame to hold the canvas
        Gtk::Frame * frame = manage (new Gtk::Frame ());
        frame->set_shadow_type (Gtk::SHADOW_IN);

        // add frame to alignment widget container
        alignment->add (*frame);

        // canvas
        m_canvas = Gtk::manage (new Gnome::Canvas::CanvasAA ());

        // set some canvas properties
        m_canvas->set_flags (Gtk::CAN_FOCUS);
        m_canvas->grab_focus ();
        m_canvas->set_size_request ((SQUARE_SIZE * 64) + 1,
                                    (SQUARE_SIZE * 64) + 1);
        m_canvas->set_scroll_region (0, 0, (SQUARE_SIZE * 64) + 1,
                                     (SQUARE_SIZE * 64) + 1);
        m_canvas->set_center_scroll_region (false);

        // add canvas to frame
        frame->add (*m_canvas);

        // board
        draw_board ();

        // pieces
        /*
         * TODO load pieces should check if the dirname exists and ends with
         * trailing slash. after concat it should check whether the file
         * exists before attempting to create a pixbuf 
         */

        if (!load_pieces ("./pixmaps/"))
        {
                Gtk::MessageDialog dialog ("ERROR: Could not load pieces");
                dialog.run ();
        }

        move_piece ("Pawn0", 0, 270 - SQUARE_SIZE);
}

board::~board ()
{
        for (std::map < std::string, Piece * >::iterator i =
             m_pieces.begin (); i != m_pieces.end (); i++)
        {
                std::cout << "* Destroyed " << i->first << std::endl;
                delete i->second;
        }
}

void
board::draw_board ()
{
        // Gtk::Table* table = manage(new Gtk::Table(8, 8, true));

        int i, j;

        for (i = 0; i < 8; i++)
        {
                for (j = 0; j < 8; j++)
                {
                        // item and slot
                        m_board[i][j] =
                                Gtk::manage (new Gnome::Canvas::
                                             Group (*m_canvas->root (),
                                                    i * SQUARE_SIZE,
                                                    j * SQUARE_SIZE));

                        // rect chess square
                        Gnome::Canvas::Rect * rect =
                                Gtk::manage (new Gnome::Canvas::
                                             Rect (*m_board[i][j], 0.0, 0.0,
                                                   SQUARE_SIZE, SQUARE_SIZE));

                        /*
                         * rgb.txt: http://www.google.com/search?q=rgb.txt 
                         */
                        rect->property_fill_color () =
                                ((i + j) % 2) ? "firebrick" : "PeachPuff3";

                        rect->property_outline_color () = "black";
                        rect->property_width_units () = 2.0;

                        // what to do when a piece is drop
                        // on top of this square?
                        /*
                         * rect->signal_event (). connect (bind (slot (*this, 
                         * &board::on_board_event), rect)); 
                         */
                        // put the square in our
                        // "database"
                        m_squares[i * 8 + j] = rect;
                }
        }
}

bool
board::on_board_event (GdkEvent * event, Gnome::Canvas::Item * item)
{
        static double x, y;
        double item_x = 0.0, item_y = 0.0;
        /*
         * FIXME double from; short to_move; 
         */
        double drop_x, drop_y;
        double new_x, new_y;

        static bool dragging;

        /*
         * If it was a square, find the piece on it 
         */
        /*
         * if (data) {
         * item = priv->db_image [GPOINTER_TO_INT (data)];
         * if (item == NULL && event->type != GDK_BUTTON_RELEASE) 
         * return 0;
         * }
         */
        switch (event->type)
        {
        case GDK_BUTTON_PRESS:
                if (event->button.button != 1)
                        break;

                x = item_x = event->button.x;
                y = item_y = event->button.y;

                // priv->orig_x = priv->curr_x = event->button.x;
                // priv->orig_y = priv->curr_y = event->button.y;

                // from = get_square (priv->curr_x,
                // priv->curr_y,
                // priv->flip);

                // if (WPIECE (priv->pos->square[from]) && priv->white_lock)
                // break;
                // if (BPIECE (priv->pos->square[from]) && priv->black_lock)
                // break;

                item->raise_to_top ();
                // gnome_canvas_item_raise_to_top (item);

                item->grab (GDK_POINTER_MOTION_MASK |
                            GDK_BUTTON_RELEASE_MASK,
                            Gdk::Cursor (Gdk::FLEUR), event->button.time);

                dragging = true;
                // priv->from = from; 
                // priv->to = from;
                // priv->moving = TRUE;
                break;

        case GDK_MOTION_NOTIFY:
                if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
                {
                        double new_x = item_x;
                        double new_y = item_y;

                        item->move (new_x - x, new_y - y);
                        x = new_x;
                        y = new_y;
                }

                if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
                {
                        new_x = event->motion.x;
                        new_y = event->motion.y;
                        item->move (new_x - x, new_y - y);

                        // if (priv->selected != NULL)
                        // clear_square (&priv->selected);

                        // assign these back to 
                        // the given piece?
                        x = new_x;
                        y = new_y;
                }

                break;

        case GDK_BUTTON_RELEASE:
                if (event->button.button != 1)
                        break;

                if (item != NULL)
                        item->ungrab (event->button.time);

                drop_x = event->button.x;
                drop_y = event->button.y;

                /*
                 * Click to move handling 
                 */
                /*
                 * priv->to = get_square (drop_x, drop_y, priv->flip);
                 * if (priv->to == priv->from && priv->selected == NULL) {
                 * mark_square (board, drop_x, drop_y);
                 * gnome_canvas_item_move (item, 
                 * priv->orig_x - priv->curr_x,
                 * priv->orig_y - priv->curr_y);
                 * priv->selected_from = priv->from;
                 * priv->moving = FALSE;
                 * break;
                 * } else if (priv->to == priv->from) {
                 * to_move = position_get_color_to_move (priv->pos);
                 * 
                 * // Unmark the selected square if it is clicked again, or mark
                 * // a different square if it contains a piece of the same color
                 * if (priv->to == priv->selected_from) {
                 * clear_square (&priv->selected);
                 * priv->from = -priv->to;
                 * priv->moving = FALSE;
                 * break;
                 * } else if (PIECE_IS (priv->pos->square[priv->to]) == to_move ) {
                 * mark_square (board, drop_x, drop_y);
                 * priv->from = priv->to;
                 * priv->selected_from = priv->to;
                 * break;
                 * } else {
                 * priv->from = priv->selected_from;
                 * }
                 * }
                 */

                /*
                 * if (try_move (board, item)) {
                 * if (priv->selected)
                 * clear_square (&priv->selected);
                 * priv->moving = FALSE;
                 * } else if (!priv->selected) {
                 * priv->moving = FALSE;
                 * }
                 */

                break;
        default:
                break;
        }
        return false;
}

double
board::snap(double n)
{
/*
                 * Snap to place 
                 */
                // logic: 226/45 = 5 (int)
                // 5 * 45 = 225 (which is our allowed value)


        double snap = int (int (n) / SQUARE_SIZE);
        snap = snap * SQUARE_SIZE;

        return snap;
}

bool
board::on_item_event (GdkEvent * event, Gnome::Canvas::Item * item)
{

        double snap_y = 0;
        double snap_x = 0;

        static double x, y, old_snap_x, old_snap_y;
        double item_x, item_y;
        static bool dragging;

        Piece *_piece = NULL;

        item_x = event->button.x;
        item_y = event->button.y;

        item->property_parent ().get_value ()->w2i (item_x, item_y);

        switch (event->type)
        {
        case GDK_BUTTON_PRESS:
                switch (event->button.button)
                {
                        // dragging with button 1
                case 1:
                        /*
                         * nice trick for deleting the item while pressing
                         * SHIFT: if (event->button.state & GDK_SHIFT_MASK) {
                         * delete item; } else { 
                         */
                        x = item_x;
                        y = item_y;
                        
                        /* 
                         * record the current position of this piece so that
                         * we can find it later if we need to
                         */
                        old_snap_x = snap(event->button.x);
                        old_snap_y = snap(event->button.y);

                        item->raise_to_top ();
                        item->grab (GDK_POINTER_MOTION_MASK |
                                    GDK_BUTTON_RELEASE_MASK,
                                    Gdk::Cursor (Gdk::FLEUR),
                                    event->button.time);
                        dragging = true;
                        // }
                        break;
                        /*
                         * case 2: if (event->button.state & GDK_SHIFT_MASK)
                         * { item->lower_to_bottom (); } else { item->lower
                         * (1); } break;
                         * 
                         * case 3: if (event->button.state & GDK_SHIFT_MASK)
                         * { item->raise_to_top (); } else { item->raise (1);
                         * } break; 
                         */
                default:
                        break;
                }

                break;

        case GDK_MOTION_NOTIFY:
                if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
                {
                        double new_x = item_x;
                        double new_y = item_y;

                        item->move (new_x - x, new_y - y);
                        /*
                         * std::cout << "new_x: " << new_x << std::endl;
                         * std::cout << "new_y: " << new_y << std::endl;
                         */
                        x = new_x;
                        y = new_y;
                }
                break;

        case GDK_BUTTON_RELEASE:
                item->ungrab (event->button.time);
                dragging = false;
                
                snap_x = snap (event->button.x);/* this is in x world
                                                 * left-top coordinate of our 
                                                 * board square */

                snap_y = snap (event->button.y);/* this is in y world
                                                 * left-top coordinate of our 
                                                 * board square */

                /*
                 * destroy piece and re-create in right place
                 */
                std::cout << "Does piece exists ? (" << old_snap_x << "," << old_snap_y << ")" << std::endl;
                _piece = get_piece(old_snap_x,old_snap_y);
                if (_piece)
                {
                        /*delete item;
                        create_piece_at (_piece->name, snap_x, snap_y);*/
                        std::cout << "event moving piece " << _piece->name << std::endl;
                        move_piece(_piece->name, snap_x, snap_y);
                }

                break;

        default:
                break;
        }

        return false;
}

Piece *
board::get_piece (const char *piece)
{
        return m_pieces[piece];
}

Piece *
board::get_piece (Gnome::Canvas::Item * item)
{
        /*
         * assuming pointers are unique numbers
         * we need to know what member in m_pieces
         * has this pointer, then get that piece name
         */
        for (std::map < std::string, Piece * >::const_iterator i =
             m_pieces.begin (); i != m_pieces.end (); i++)
        {
                // FIXME these pointers don't match ?
                if (i->second->pixbuf == item)
                {

                        std::cout << "board::get_piece(Item*) piece found: " << i->second->name << std::endl;
                        return m_pieces[i->second->name];
                }
        }
        std::cout << "board::get_piece(Item*) no piece found" << std::endl;
        return NULL;
}

Piece *
board::get_piece (double col, double row)
{
        /*
         * assuming pointers are unique numbers
         * we need to know what member in m_pieces
         * has this pointer, then get that piece name
         */
        for (std::map < std::string, Piece * >::const_iterator i =
             m_pieces.begin (); i != m_pieces.end (); i++)
        {

                //std::cout << "board::get_piece(double,double) comparing: " << i->second->name << "(" << i->second->x << "," << i->second->y << ")" << std::endl;
                if (i->second->x == col && i->second->y == row)
                {

                        std::cout << "board::get_piece(double,double) piece found: " << i->second->name << std::endl;
                        return m_pieces[i->second->name];
                }
        }
        std::cout << "board::get_piece(double,double) no piece found at (" << col << "," << row << ")" << std::endl;
        return NULL;
}

/*
 * moves piece from square 1 to square 2 without displaying motion (snap)
 */
bool
board::move_piece (const char *name, double col, double row)
{

		 const char *col_names[] = {
                "a",
                "b",
                "c",
                "d",
                "e",
                "f",
                "g",
                "h",
                NULL
        };
        
    		std::string move_name = "";
        
        if (name [0] != 'p' && name [0] != 'P'){
        		move_name += name [0];
        		
        		}
        	
       			move_name += col_names[int(col)/45];  			
        		
        		// Only what i have founded to convert int into string
        		std::stringstream ss;
				std::string str;
				ss << (int(8 - row/45));
				ss >> str;
				
				move_name += str;
        		//std::cout << name [0] << col_names[int(col)/45] << 8 - row/45 << std::endl;  
        		std::cout << move_name << std::endl;
        	   
        	   
        
        std::cout << "board::move_piece(" << name << "," << col << "," << row
                << ")" << std::endl;
        if (!m_pieces[name])
        {
                std::cout << "missing struct?" << std::endl;
                return false;
        }
        if (m_pieces[name]->deleted)
        {
                std::cout << "piece deleted?" << std::endl;
                return false;
        }
        /*
         * TODO validate item: is it valid? are we already in that same
         * square? capture? we should update the game log 
         */

        std::cout << "deleting " << name << " " << m_pieces[name]->
                file_path << std::endl;
        delete m_pieces[name]->pixbuf;
        m_pieces[name]->deleted = true;

        std::cout << "deleted " << name << " " << m_pieces[name]->
                file_path << std::endl;

        if (create_piece_at (name, col, row))
        {

                std::cout << "created " << name << " " << m_pieces[name]->
                        file_path << std::endl;

                m_pieces[name]->pixbuf->raise_to_top ();
                std::cout << name << " moved to: (" << col << "," << row <<
                        ")" << std::endl;

                m_pieces[name]->deleted = false;
                return true;
        }
        else
        {
                std::cout << "could not create " << name << " " <<
                        m_pieces[name]->file_path << std::endl;
        }
        return false;
}

/*
 * pieces Creating a darng piece from within the class pieces and then
 * connecting it to the propper slot for dragging doesn't work. Thus,
 * brought back to here.
 * 
 * This function creates a picture buffer and puts it in the canvas pass to
 * it as "root" after conecting it to a given signal slot.
 * 
 * Then returns the Gnome::Canvas::Pixbuf as this is the Gnome::Canvas::Item 
 * that we will need to move() from our network-related class
 * 
 */

Gnome::Canvas::Pixbuf *
        board::create_piece (const char *image_file, double col, double row)
{
        Glib::RefPtr < Gdk::Pixbuf > image =
                Gdk::Pixbuf::create_from_file (image_file);
        Gnome::Canvas::Pixbuf * pixbuf = NULL;

        if (image)
        {
                pixbuf = Gtk::manage (new Gnome::Canvas::
                                      Pixbuf (*m_canvas->root (), col, row,
                                              image));

                pixbuf->signal_event ().
                        connect (bind
                                 (SigC::slot (*this, &board::on_item_event),
                                  pixbuf));

                pixbuf->raise_to_top ();

                /*
                 * #ifdef _DEBUG Gtk::MessageDialog dialog("Success: Piece
                 * created"); dialog.run(); #endif 
                 */
        }
        else
        {
                // else { DIE BASTARD! LOL }
#ifdef _DEBUG
                Gtk::MessageDialog
                        dialog ("Error: Piece could not be created");
                dialog.run ();
#endif
        }
        // debug_msg("Any successful strings before this?");
        return pixbuf;
}

bool
board::load_pieces (const char *path)
{
        // prefs_get_piecedir ()

        /*
         * Pieces: names order matters, it has to match the order of the
         * piece files array below 
         */
        const char *names[] = {
                "Pawn",
                "Night",
                "Bishop",
                "Rook",
                "Queen",
                "King",
                "pawn",
                "night",
                "bishop",
                "rook",
                "queen",
                "king",
                NULL
        };

        /*
         * Pieces: files See past note on names 
         */
        char *files[] = {
                "P.png",
                "N.png",
                "B.png",
                "R.png",
                "Q.png",
                "K.png",
                "p.png",
                "n.png",
                "b.png",
                "r.png",
                "q.png",
                "k.png",
                NULL
        };

        // double row = 0.0;
        // double col = 0.0;

        for (int i = 0; files[i] != NULL; i++)
        {
                /*
                 * TODO: Some day I should find a better way of dealing with
                 * this mess 
                 */
                char *fname;
                fname = g_strconcat (path, files[i], NULL);

                if (names[i] == "Pawn" || names[i] == "pawn")
                {
                        create_pawn (names[i], fname);
                }
                else if (names[i] == "Night" || names[i] == "night")
                {
                        create_knight (names[i], fname);
                }
                else if (names[i] == "Bishop" || names[i] == "bishop")
                {
                        create_bishop (names[i], fname);
                }
                else if (names[i] == "Rook" || names[i] == "rook")
                {
                        create_rook (names[i], fname);
                }
                else if (names[i] == "Queen" || names[i] == "queen")
                {
                        create_queen (names[i], fname);
                }
                else if (names[i] == "King" || names[i] == "king")
                {
                        create_king (names[i], fname);
                }

                /*
                 * m_pieces[names[i]] = create_piece (fname,
                 * (col + j) *
                 * SQUARE_SIZE, 
                 * row *
                 * SQUARE_SIZE,
                 * );
                 */

                delete fname;
        }                       // end for files

        // TODO: fix fix fix... should I throw/catch?
        // if pieces are ok, return true, else ... 
        return true;
}

/**
 * wrapper to make it easier to place pieces inside a board square
 *
 * @param name proper name for piece, like pawn0, Pawn0 for black or white pawn, respectively
 * @param col Gnome::Canvas world coordinate column to place piece
 * @param row Gnome::Canvas world coordinate row to place piece
 */
bool board::create_piece_at (const char *name, double col, double row)
{
        if (!m_pieces[name])
        {
                std::cout <<
                        "Cannot create piece. struct missing"
                        << std::endl;
                return false;
        }

        if (!g_file_test (m_pieces[name]->file_path, G_FILE_TEST_IS_REGULAR))
        {
                std::cerr << "Could not open file " << m_pieces[name]->
                        file_path << std::endl;
                return false;
        }

        // make sure we are in the right position
        m_pieces[name]->x = snap(col);
        m_pieces[name]->y = snap(row);
        /*std::cout << "board::create_piece_at (const char *name, double col, double row) " <<
                "col changed from " << col << " to " << m_pieces[name]->x << std::endl;
        std::cout << "board::create_piece_at (const char *name, double col, double row) " <<
                "row changed from " << row << " to " << m_pieces[name]->y << std::endl;*/

        /*
         * store name to make it easier to find later (programmatically)
         */
        if (!m_pieces[name]->name || m_pieces[name]->name < "")
                m_pieces[name]->name = g_strdup (name);
        
        /*
         * create piece and store it 
         */
        std::cout << "+ creating piece: " << name << " (" << col << "," << row
                << ")" << std::endl;
        m_pieces[name]->pixbuf =
                create_piece (m_pieces[name]->file_path, m_pieces[name]->x,
                              m_pieces[name]->y);

        m_pieces[name]->deleted = false;

        if (m_pieces[name]->pixbuf)
                return true;
        return false;
}

/**
 * wrapper to make it easier to place pieces inside a board square
 *
 * @param _piece piece struct (populated)
 */
bool board::create_piece_at (Piece * _piece)
{
        /* 
         * do we care if piece is in m_pieces or not? 
         * this function will be called like: board::create_piece_at(m_pieces[_name]) anyway...
         * leaving this check here for now:
         */
        if (!m_pieces[_piece->name])
        {
                std::cout <<
                        "Cannot create a piece that already exist or struct missing"
                        << std::endl;
                return false;
        }

        if (!g_file_test (_piece->file_path, G_FILE_TEST_IS_REGULAR))
        {
                std::cerr << "Could not open file " << _piece->
                        file_path << std::endl;
                return false;
        }

        _piece->x = snap(_piece->x);
        _piece->y = snap(_piece->y);

        std::cout << "+ creating piece: " << _piece->name << " (" << _piece->x << "," << _piece->y 
                << ")" << std::endl;

        /*
         * create piece and store it 
         */
        _piece->pixbuf =
                create_piece (_piece->file_path, _piece->x, _piece->y);

        _piece->deleted = false;

        if (_piece->pixbuf)
                return true;
        return false;
}

/**
 * creates all the white or black pawns depending on the
 * name given. the pawns will have their original positions
 */
void
board::create_pawn (const char *name, const char *file_path)
{
        double row = 0.0;
        double col = 0.0;
        char * _name = NULL;
        if (name == "Pawn")
        {
                // white
                row = 6.0;
        }
        else
        {
                row = 1.0;
        }

        for (int j = 0; j < 8; j++)
        {
                // pawn0, pawn1, ..., pawnN
                _name = g_strdup_printf ("%s%d", name, j);
                m_pieces[_name] = new Piece;
                m_pieces[_name]->file_path = g_strdup (file_path);
                m_pieces[_name]->name = g_strdup (_name);
                m_pieces[_name]->x = (col + j) * SQUARE_SIZE;
                m_pieces[_name]->y = row * SQUARE_SIZE;

                create_piece_at (m_pieces[_name]);
        }                       // end for
}                               // end create_pawn

void
board::create_knight (const char *name, const char *file_path)
{
        double row = 0.0;
        double col = 1.0;
        char * _name = NULL;
        
        if (name == "Night")
        {
                // white
                row = 7.0;
        }

        /*
         * we have 2 knights 
         */
        _name = g_strdup_printf ("%s%d", name, 0);
        m_pieces[_name] = new Piece;
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->name = g_strdup (_name); // dup because _name is freed in this scope
        m_pieces[_name]->x = col * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;

        create_piece_at (m_pieces[_name]);

        _name = g_strdup_printf ("%s%d", name, 1);
        m_pieces[_name] = new Piece;
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->x = (col + 5.0) * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;

        create_piece_at (m_pieces[_name]);
}                               // end create_knight

void
board::create_bishop (const char *name, const char *file_path)
{
        double row = 0.0;
        double col = 2.0;
        char *_name = NULL;
        
        if (name == "Bishop")
        {
                // white
                row = 7.0;
        }

        /*
         * we have 2 bishops 
         */

        _name = g_strdup_printf ("%s%d", name, 0);
        m_pieces[_name] = new Piece;
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->x = col * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;

        create_piece_at (m_pieces[_name]);

        _name = g_strdup_printf ("%s%d", name, 1);
        m_pieces[_name] = new Piece;
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->x = (col + 3.0) * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;

        create_piece_at (m_pieces[_name]);
}                               // end create_bishop

void
board::create_rook (const char *name, const char *file_path)
{
        double row = 0.0;
        double col = 0.0;
        char *_name = NULL;

        if (name == "Rook")
        {
                // white
                row = 7.0;
        }

        /*
         * we have 2 rooks 
         */
        _name = g_strdup_printf ("%s%d", name, 0);
        m_pieces[_name] = new Piece;
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->x = col * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;
        create_piece_at (m_pieces[_name]);

        _name = g_strdup_printf ("%s%d", name, 1);
        m_pieces[_name] = new Piece;
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->x = (col + 7.0) * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;
        create_piece_at (m_pieces[_name]);
}                               // end create_rook

void
board::create_queen (const char *name, const char *file_path)
{
        double row = 0.0;
        double col = 3.0;
        const char *_name = g_strdup_printf ("%s%d", name, 0);

        if (name == "Queen")
        {
                // white
                row = 7.0;
        }

        m_pieces[_name] = new Piece;
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->x = col * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;
        create_piece_at (m_pieces[_name]);
}                               // end create_queen

void
board::create_king (const char *name, const char *file_path)
{
        double row = 0.0;
        double col = 4.0;
        const char *_name = g_strdup_printf ("%s%d", name, 0);

        if (name == "King")
        {
                // white
                row = 7.0;
        }

        m_pieces[_name] = new Piece;
        m_pieces[_name]->file_path = g_strdup (file_path);
        m_pieces[_name]->name = g_strdup (_name);
        m_pieces[_name]->x = col * SQUARE_SIZE;
        m_pieces[_name]->y = row * SQUARE_SIZE;

        create_piece_at (m_pieces[_name]);
}                               // end create_king