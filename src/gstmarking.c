#include "gstmarkin.h"
#include "gstmarkout.h"

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
marking_plugin_init (GstPlugin * marking_plugin)
{
	return gst_element_register(marking_plugin, "markin", GST_RANK_NONE, GST_TYPE_MARKIN) &&
			gst_element_register(marking_plugin, "markout", GST_RANK_NONE, GST_TYPE_MARKOUT);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "marking"
#endif

GST_PLUGIN_DEFINE (
		GST_VERSION_MAJOR,
		GST_VERSION_MINOR,
		marking,
		"Timestamping for performance measurement",
		marking_plugin_init,
		"1.0.0",
		"LGPL",
		"Rainey",
		"http://raineyelectronics.com/"
)
