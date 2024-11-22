import React, { forwardRef } from "react";

const ContextMenus = forwardRef<HTMLUListElement, any>((props, ref) => {
  const menuClick = (e: any) => {
    console.log("You clicked on '" + e.target.textContent + "'");
  };

  const MenuOptions = [
    { id: 1, name: "Activate" },
    { id: 2, name: "Show" },
    { id: 3, name: "Delete" },
  ];

  return (
    <ul
      ref={ref}
      className="absolute w-36 bg-gray-600 text-white rounded-lg mt-3 p-2"
      style={{ top: props.position.y, left: props.position.x }}
    >
      {MenuOptions.map((option) => (
        <li key={option.id} onClick={menuClick} className="text-start cursor-pointer hover:bg-gray-700 px-2 py-1">
          {option.name}
        </li>
      ))}
    </ul>
  );
});

export default ContextMenus;
