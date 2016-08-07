#ifndef HAVE_FOO_H
#define HAVE_FOO_H

const SecretSchema* foo_get_schema(void) G_GNUC_CONST;

#define FOO_SCHEMA (foo_get_schema())

#endif

