GStreamer elements that use the new GstMeta API to allow for performance measurement of GStreamer pipelines.

Sample usage:

export GST_DEBUG=markout:5

gst-launch-1.0 -f -e videotestsrc pattern=ball ! video/x-raw,width=320,height=240 ! markin name=moo !  videoscale ! markout ! video/x-raw,width=1280,height=720 ! ximagesink
