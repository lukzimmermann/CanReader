import { X } from "@phosphor-icons/react";
import { useEffect } from "react";
import { Link } from "react-router-dom";
import { useLocation } from "react-router-dom";

interface LinkItem {
  name: string;
  url: string;
}

type Props = {
  isOpen: boolean;
  onCloseClick: () => void;
};

function MenuBar({ onCloseClick, isOpen }: Props) {
  const location = useLocation();
  const linkClasses = "pl-5 py-2 w-full text-start rounded-lg hover:bg-purple-800 active:bg-purple-900";

  useEffect(() => {
    console.log(location);
    console.log("fuck");
  }, []);

  const linkList: LinkItem[] = [
    { name: "Dashboard", url: "/" },
    { name: "Channels", url: "/channels" },
    { name: "DBC-File", url: "/dbc" },
  ];

  return (
    <div
      className={`fixed top-0 left-0 h-full w-64 bg-gray-700 text-white transform ${
        isOpen ? "translate-x-0" : "-translate-x-full"
      } transition-transform duration-300 ease-in-out z-50`}
    >
      <div className="flex justify-between items-center p-5 border-b border-gray-800">
        <h2 className="text-lg font-bold pl-5">Menu</h2>
        <button onClick={onCloseClick} className="text-white">
          <X size={24} />
        </button>
      </div>
      <div className="flex flex-col items-start p-5 gap-2">
        {linkList.map((item) => (
          <Link
            key={item.name}
            className={`${linkClasses} ${item.url === location.pathname ? "bg-purple-700" : ""}`}
            to={item.url}
            onClick={onCloseClick}
          >
            {item.name}
          </Link>
        ))}
      </div>
    </div>
  );
}

export default MenuBar;
