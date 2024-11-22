import { useEffect, useRef, useState } from "react";
import ContextMenus from "./contextMenus";

type Props = {
  files: string[];
};

function FileList({ files }: Props) {
  const [showRightMenu, setShowRightMenu] = useState(false);
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const menuRef = useRef<HTMLUListElement | null>(null);

  useEffect(() => {
    // Close the context menu if clicking outside of it
    const handleClickOutside = (e: MouseEvent) => {
      if (menuRef.current && !menuRef.current.contains(e.target as Node)) {
        hideContextMenu();
      }
    };

    // Attach the event listener when the context menu is shown
    if (showRightMenu) {
      document.addEventListener("mousedown", handleClickOutside);
    }

    // Clean up the event listener when the component is unmounted or context menu is hidden
    return () => {
      document.removeEventListener("mousedown", handleClickOutside);
    };
  }, [showRightMenu]);

  const contextMenu = (e: any) => {
    e.preventDefault();
    setPosition({ x: e.pageX, y: e.pageY });
    setShowRightMenu(true);
  };

  const hideContextMenu = () => {
    setShowRightMenu(false);
  };

  return (
    <div onClick={hideContextMenu}>
      {files.map((item) => (
        <div
          className="p-2 border-b border-gray-600 cursor-pointer hover:bg-gray-600 last:border-b-0"
          onContextMenu={contextMenu}
        >
          <span className="text-start">{item}</span>
        </div>
      ))}
      {showRightMenu && <ContextMenus ref={menuRef} position={position} />}
    </div>
  );
}

export default FileList;
