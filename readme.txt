=== Artistography ===
Contributors: MistahWrite
Tags: artistography, artist, discography, media, music, videos, images, pictures, gallery, galleries, songs, albums, eCommerce, e-Commerce, PayPal, soundmanager2, playlist, cart, download manager, ftp, zupload, encode explorer, store, checkout 
Requires at least: 3.0.1
Tested up to: 4.1
Stabe tag: 0.3.3-alpha
License: GPLv2 or later
License URI: http://www.gnu.org/licenses/gpl-2.0.html

Organizes a portfolio of music, videos, and images on your blog/website with PayPal eCommerce.

== Description ==

Allows you to organize music, pictures/images, and videos on an artist page.  The artist page has jQuery-UI style tabs, and polls tags on posts for the artist's name and then files them into the Related Posts tab.  It utilizes soundmanager2 and displays free songs on a player on the website.  Recommended for use with Advanced AJAX Page Loader.  This is a constant work in progress, and is evolving into an incredible plugin for musicians or record labels.

Uses for FTP and file browsing capability:
 * http://sourceforge.net/projects/encode-explorer/ (version 6.3)
 * http://sourceforge.net/projects/zupload/ 

== Installation ==

1. Install the Artisography plugin to your blog, and Activate it.  
2. Admin/Dashboard > Artistography > Artists: Create An Artist
3. Admin/Dashboard > Artistography > Music Albums: Create An Album
4. Admin/Dashboard > Artistography > Artist/Album Linker: Drag and Drop the Artist to the Music Album
5. To populate the artist's Related Posts tab just tag posts with the Artist's name.  Artistography will automatically find it.
6. Under Appearance > Widgets use the "Artistography Shopping Cart" and "Artistography Products" widgets as needed.
7. Use the following shortcodes on a page/post:

[artistography_display_artist_page id=1]
     Displays photo/bio, discography, videos, and related posts of artist with database id=1

[artistography_artist_name id=1]
     Displays the name of the artist with database id=1.

[artistography_display_artists]
     Displays artists in a table with pictures that break away on mouse-over
          Optional paramaters:
               cols=(default 4)

[artistography_display_artist]

[artistography_playlist id=1]
	Displays songs that are linked to album with id=1 in a list ordered by the song track numbers

[artistography_album_download_link id=1]
     Displays a download picture with link to download album of album with database id=1

[artistography_album_art id=1]
     Displays album photo for album with database id=1

[artistography_display_albums]
     Displays albums in a table with associated download hyperlink (if linked to a download) or add to cart link
          Optional paramters:
		cols=(default 3)
		size=(default 200)

[artistography_display_album id=1]
     Displays album where id=1 with associated download hyperlink (if linked to a download) or add to cart link
	Optional parameters:
		cols=(default 3)
		size=(default 200)

[artistography_display_album_tracklist id=1]
     Displays tracklist/description of album with album database id=1
[artistography_display_album_artist id=1]
     Displays artist name of album with album database id=1

== Changelog ==

=0.3.3-alpha=
*Fixed bug where soundmanager2 bar-ui wasn't showing up unless logged in
*Playlist dynamically updates on play button press... Loads song/album into bar-ui
*Does play links that you click, but still unstable with known defects regarding not being able to pause/stop playback when initiating playback from artistography methods

=0.3.2=
*AJAX Shopping Cart, removed standard post method

=0.3.2-alpha=
*Fixed a ton of bugs/notices/warnings indicated by WP_DEBUG
*Added Products widget
*Fixed shopping cart/checkout cart where more than one item does not show up
*Fixed shopping cart where quantity of any download will never exceed 1
*Updated small section of the About page

=0.3.1=
*Added shopping cart widget
*Added loadAlbumArtRefresh() to update album art when menu button is pressed on soundmanager2

=0.3.1-alpha5=
*Add [artistography_playlist] shortcode
*Fixed shuffle of soundmanager2 playlist items

=0.3.1-alpha4=
*Show album art in soundmanager2 playlist

=0.3.1-alpha3=
*Added song/album linker

=0.3.1-alpha2=
*Added thickbox

=0.3.1-alpha=
*Added AJAX Loader Option in case using Advanced AJAX Page Loader, Wordpress AJAXIFY or the like to include scripts/styles on all pages so they are there if needed

=0.3.0=
*Randomize soundmanager2 playlist order
*Added stylesheet for soundmanager2

=0.3.0-alpha5=
*Properly displays explicit tag in soundmanager2 player

=0.3.0-alpha4=
*Added Track number/explicit fields to song class

=0.3.0-alpha3=
*Fixed some minor bugs breaking release

=0.3.0-alpha2=
*Added soundmanager v2.97
*Fixed minor bug in Music Manager related to not getting next node for Download selector
*Added Songs class/admin manager
*Free songs load into soundmanager playlist to play on the site
*Fixed artists display script

=0.2.8=
*Fixed Updated/Edit Gallery Posts
*Fixed Order manager table colspan

=0.2.8-alpha10=
*AJAX Music Manager
*Orders page now has filter and AJAX Viewer
*Worked on Gallery Edit

=0.2.8-alpha9=
*Dialog window to view orders in detail

=0.2.8-alpha8=
*Added parent_txn_id to orders table

=0.2.8-alpha7=
*Added most of the paypal options we may need for a full store later

=0.2.8-alpha5=
*Updated Orders class to support new database options

=0.2.8-alpha3=
*Updated Orders admin panel

=0.2.8-alpha2=
*Changed database for orders and tweaked to get it working
*Added Business Name option

=0.2.7=
*Using curl for PayPal IPN and reading raw POST data instead

=0.2.6=
*Updated small bugs in cart

=0.2.3=
*Changed jQuery for Artist picture hover to fadeIn fadeOut and added Image Gallerie to stats
*Working on PayPAl IPN

=0.2.2=
*Still working on Gallery manager

=0.2.1-alpha5=
*Working on Gallery Manager
*Added Lightbox/Colorbox Gallery Styles

=0.2.1-alpha4=
*Got Gallery Manager working

=0.2.1-alpha3=
*Setup add to cart/download link shower to display download link if logged in user paid for it already
*Pass IPN URL permalink to PayPal
*Add artist photos from media library
*Added Image Galleries tab/code
*Added specific script adder to use separate .js files per admin page

=0.2.1-alpha2=
*Fixed bug in IPN handler
*Added User ID to Orders admin interface table

=0.2.1-alpha=
*Fixed bug in insert music album script preventing music from being added

=0.2.1=
*Added security and Orders processed checking to Downloads

=0.2.0=
*Finished Orders class and Orders tab fully functional
*Added PayPal Sandbox for testing option to Options menu

=0.1.9=
*Added price to albums and removed enable state, featured state, and free downloads enabled state
*Added IPN, Orders, and Thank You pages for ordering and hid them
*Updated Cart, Checkout, Orders, and Thank you for ordering pages to have no comments/pingbacks
*Added session_start() to main plugin file to resume $_SESSION
*Added "Add to Cart" images to music instead of Download image link when price is not free
*Got Cart/Checkout working
*Added Orders Admin menu
*Completed Instant Payment Notifications (IPN) code

=0.1.8=
*Add Sale class
*Working on Cart/Checkout system

=0.1.7=
*Created cart and checkout page and hid them
*Fixed download script to point to the proper directory

=0.1.6=
*Fixed copy explorer to downloads path

=0.1.5=
*Updated FTP Uploader and downloads directory because downloads directory in plugin folder is deleted upon update

=0.1.4=
*Fixed directory to files
*Changed FTP Password to Password textbox
*Fixed Downloads admin interface

=0.1.3=
*Fixed FTP Uploader ... Misplaced semi-colon broke page in last version
*Fixed enqueue scripts/styles directories
*Added Download class/undocumented shortcode
*Changed Download URL handler to utilize shortcode and Download class
*Added PayPal Donation Email option for download page

=0.1.2=
* Fixed script logic on deactivate/uninstall to preserve_database

=0.1.1=
* Fixed options screen
* Used preserve_database option in uninstall
* Fixed delete track_list database table
* Fixed install script logic

=0.1.0=
* Added ZUpload from sourceforge instead of unlimited_ftp
* Added eXtplorer 2.1.5 from sourceforge for PHP file browser
* Fixed options screen

=0.0.9=
* Removed track_list database and support routines
* Removed stats page
* Moved Options page towards the bottom
* Made textbox into textarea for artist description
* Fixed FTP Uploader page with plugins_url()

=0.0.8=
* Updated style for admin panels
* Worked on AJAX Artist Create, Edit, Update, and Delete Artist
* Dropped artist birthday from database
* Dropped support for regular post methods for artist create, edit, and update
* Preparing for full internationalization support
* Finally using $artist->incrementPageViewsById($id) when shortcode is used: [artistography_display_artist_page id=x]
* Displaying artist page views in manage artist admin panel

=0.0.7-alpha=
* Fixed Related Posts size so that accordions would be visible

= 0.0.7 =
* Split up css for admin and display site

= 0.0.6 =
* Added function: artistography_enqueue_admin_style_and_scripts()

= 0.0.5 =
* Fixed broken css/javascript include path: artistography_enqueue_style_and_scripts

= 0.0.4 =
* More internationalization support.

= 0.0.3 =
* Removed myspace_url support from artist database table
* Added function: artistography_enqueue_style_and_scripts()

= 0.0.2 =
* Internationalization support begun
* Updated jQuery and jQuery-UI to version 1.11.2

= 0.0.1 =
* Created Project "Artistography"
* Setup Top-Level Menu in the Admin panel, with submenus
