module: dylan-user

define library hello
  use common-dylan;
  use io;
end library;

define module hello
  use common-dylan, exclude: { format-to-string };
  use format-out;
end module;
