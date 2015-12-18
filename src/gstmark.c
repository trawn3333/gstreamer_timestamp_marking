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

#include "gstmark.h"

GType
gst_meta_marking_api_get_type (void)
{
  static volatile GType type;
  static const gchar *tags[] = { NULL };

  if (g_once_init_enter (&type)) {
    GType _type = gst_meta_api_type_register ("GstMetaMarkingAPI", tags);
    g_once_init_leave (&type, _type);
  }
  return type;
}

gboolean
gst_meta_marking_init(GstMeta *meta, gpointer params, GstBuffer *buffer)
{
	GstMetaMarking* marking_meta = (GstMetaMarking*)meta;

	marking_meta->in_timestamp = GST_CLOCK_TIME_NONE;

	return TRUE;
}

gboolean
gst_meta_marking_transform (GstBuffer *dest_buf,
                             GstMeta *src_meta,
                             GstBuffer *src_buf,
                             GQuark type,
                             gpointer data) {
	GstMeta* dest_meta = GST_META_MARKING_ADD(dest_buf);

	GstMetaMarking* src_meta_marking = (GstMetaMarking*)src_meta;
	GstMetaMarking* dest_meta_marking = (GstMetaMarking*)dest_meta;

	dest_meta_marking->in_timestamp = src_meta_marking->in_timestamp;

	return TRUE;
}

void
gst_meta_marking_free (GstMeta *meta, GstBuffer *buffer) {
}

const GstMetaInfo *
gst_meta_marking_get_info (void)
{
  static const GstMetaInfo *meta_info = NULL;

  if (g_once_init_enter (&meta_info)) {
    const GstMetaInfo *meta =
        gst_meta_register (gst_meta_marking_api_get_type (), "GstMetaMarking",
        sizeof (GstMetaMarking), (GstMetaInitFunction)gst_meta_marking_init,
        (GstMetaFreeFunction)gst_meta_marking_free, (GstMetaTransformFunction) gst_meta_marking_transform);
    g_once_init_leave (&meta_info, meta);
  }
  return meta_info;
}
