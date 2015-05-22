#!/system/bin/sh

ACTION_SCRIPTS=/res/lpowercc/actions
source /res/lpowercc/lpowercc-helper

read_defaults

read_config

case "${1}" in
  rename)
    rename_profile "$2" "$3"
    exit 0
    ;;
  delete)
    delete_profile "$2"
    exit 0
    ;;
  select)
    select_profile "$2"
    exit 0
    ;;
  dump)
    dump_profile "$2"
    exit 0
    ;;
  config)
    print_config
    ;;
  list)
    list_profile
    ;;
  apply)
    apply_config
    ;;
  *)
    . ${ACTION_SCRIPTS}/$1 $1 $2 $3 $4 $5 $6
    ;;
esac;

write_config
