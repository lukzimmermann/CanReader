import { List } from "@phosphor-icons/react";

type Props = {
  onMenuClick: () => void;
};

function Header({ onMenuClick }: Props) {
  return (
    <div className="w-screen bg-gray-600 mb-5 p-2 sticky top-0 z-50 flex items-center justify-between">
      <button onClick={onMenuClick} className="text-white hover:bg-gray-500 p-2 rounded-xl">
        <List size={24} />
      </button>
      <h1 className="text-white text-xl font-bold text-center flex-grow">CanReader</h1>
      <div className="w-8"></div> {/* Placeholder for spacing */}
    </div>
  );
}

export default Header;
