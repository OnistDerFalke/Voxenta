if(VOXENTA_USE_GTK_FILE_PICKER)
    set(NFD_PORTAL OFF CACHE BOOL "Use Portals for Linux file dialogs" FORCE)
else()
    set(NFD_PORTAL ON CACHE BOOL "Use GTK for Linux file dialogs" FORCE)
endif()

CPMAddPackage(
        NAME nfd
        URL https://github.com/btzy/nativefiledialog-extended/archive/17b6e8ce219c0677f94b63636abb9296b28841ca.tar.gz
        URL_HASH SHA256=eba0c27a7d20ea6632a7e9788f1eb7d1ba757459709ade4b567c29b7eb78ec55
        OPTIONS "NFD_PORTAL ${NFD_PORTAL}"
)
