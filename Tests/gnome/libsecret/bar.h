#ifndef HAVE_BAR_H
#define HAVE_BAR_H

const SecretSchema* bar_get_schema(void) G_GNUC_CONST;

#define BAR_SCHEMA (bar_get_schema())

#endif

