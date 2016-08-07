#!/bin/sh

# 
# imacros.sh
# A Helper for Creating a GObject interface
# 

NOR=$1
SML=$2
BG1=$3
BG2=$4

if [ "$*" = "" ]
then
    echo "for example:"
    echo "Your class is GtkTextView."
    echo "GtkTextView   -> \$1."
    echo "gtk_text_view -> \$2"
    echo "GTK           -> \$3"
    echo "TEXT_VIEW     -> \$4"
    echo ""
    echo "Made by Mike Manilone"
    exit 0
fi

echo ""
echo "/* made with imacros.sh */"
echo "#include <glib-object.h>"
echo ""
echo "G_BEGIN_DECLS"
echo ""
echo "#define ${BG1}_TYPE_${BG2}             (${SML}_get_type ())"
echo "#define ${BG1}_${BG2}(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), ${BG1}_TYPE_${BG2}, ${NOR}))"
echo "#define ${BG1}_IS_${BG2}(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), ${BG1}_TYPE_${BG2}))"
echo "#define ${BG1}_GET_${BG2}_INTERFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), ${BG1}_TYPE_${BG2}, ${NOR}Iface))"
echo ""
echo "typedef struct _${NOR}     ${NOR};"
echo "typedef struct _${NOR}face ${NOR}Iface;"
echo ""
echo "struct _${NOR}Iface"
echo "{"
echo "};"
echo ""
echo "GType ${SML}_get_type (void);"
echo ""
echo "G_END_DECLS"

