/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2015  <mpekar@raineyelectronics.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-filter
 *
 * Add timestamp marker to buffer as metadata, to be used in conjunction with `markout`.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch videotestsrc ! markin ! jpegenc ! markout ! multifilesink location=/tmp/%d.jpg
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/base/gstbasetransform.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "gstmarkin.h"
#include "gstmark.h"

GST_DEBUG_CATEGORY_STATIC (gst_markin_debug);
#define GST_CAT_DEFAULT gst_markin_debug

/* Filter signals and args */
enum
{
	/* FILL ME */
	LAST_SIGNAL
};

enum
{
	PROP_0
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
		GST_PAD_SINK,
		GST_PAD_ALWAYS,
		GST_STATIC_CAPS_ANY
);

static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE ("src",
		GST_PAD_SRC,
		GST_PAD_ALWAYS,
		GST_STATIC_CAPS_ANY
);


#define gst_markin_parent_class parent_class
G_DEFINE_TYPE (Gstmarkin, gst_markin, GST_TYPE_BASE_TRANSFORM);

static void gst_markin_set_property (GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_markin_get_property (GObject * object, guint prop_id, GValue * value, GParamSpec * pspec);
static void gst_markin_finalize (GObject * object);
static GstFlowReturn gst_markin_transform_ip (GstBaseTransform * trans, GstBuffer * buf);


/* initialize the markins's class */
static void
gst_markin_class_init (GstmarkinClass * klass)
{
	GST_DEBUG_CATEGORY_INIT (gst_markin_debug, "markin", 0, "Timestamp marking");

	GObjectClass *gobject_class = (GObjectClass *) klass;
	GstBaseTransformClass *trans_class = (GstBaseTransformClass *) klass;

	gobject_class->set_property = gst_markin_set_property;
	gobject_class->get_property = gst_markin_get_property;
	gobject_class->finalize = gst_markin_finalize;
/*
	g_object_class_install_property (gobject_class, PROP_DISABLE_FILTER_AT_BUFFER_INDEX,
			g_param_spec_int64 ("disable-filter-at-buffer-index", "disable filter at index", "When this buffer index arrives, stop filtering.  Filters all by default",
					-1, G_MAXINT64, G_MAXINT64, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
*/

	GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

	gst_element_class_set_static_metadata(element_class,
			"filter",
			"Generic",
			"Mark in timestamp",
			"<mpekar@raineyelectronics.com>");

	gst_element_class_add_pad_template (element_class,
			gst_static_pad_template_get (&src_template));
	gst_element_class_add_pad_template (element_class,
			gst_static_pad_template_get (&sink_template));

	trans_class->transform_ip = GST_DEBUG_FUNCPTR(gst_markin_transform_ip);
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_markin_init (Gstmarkin * element)
{
  gst_base_transform_set_passthrough((GstBaseTransform*)element, TRUE);
	/*
	element->disable_filter_at_buffer_index = G_MAXINT64;
	element->filter_if_not_mod_equals = 0;
	element->pass_every_milliseconds = 0;
	element->pass_limit = -1;
	element->last_pass_time = 0;
	*/
}

static GstFlowReturn
gst_markin_transform_ip (GstBaseTransform * trans, GstBuffer * in)
{
    if(! gst_buffer_is_writable(in))
	{
	    return GST_FLOW_OK;
	}
	
	GstMetaMarking* meta = GST_META_MARKING_ADD(in);

	GstClock* clock = gst_system_clock_obtain ();
	meta->in_timestamp = gst_clock_get_time(clock);
	gst_object_unref(clock);

	return GST_FLOW_OK;
}

static void
gst_markin_set_property (GObject * object, guint prop_id,
		const GValue * value, GParamSpec * pspec)
{
	switch (prop_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gst_markin_get_property (GObject * object, guint prop_id,
		GValue * value, GParamSpec * pspec)
{
	switch (prop_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gst_markin_finalize (GObject * object)
{
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

