import { CanMessage } from "../../interfaces/can";

type Props = {
  data: CanMessage;
};

function CanMessageItem({ data }: Props) {
  return (
    <div className="flex py-3 border-t border-t-slate-600 hover:bg-slate-700 text-sm sm:text-base">
      <div className="flex flex-col text-left pl-2 gap-1">
        <span>ID:</span>
        <span>Length:</span>
        <span>Frequency:</span>
        <div className="flex gap-2">
          <div
            className={`flex rounded-full justify-center w-6 h-6 pt-0.5 text-sm ${
              data.isExtended ? "bg-violet-600" : "bg-gray-500"
            }`}
          >
            R
          </div>
          <div
            className={`flex rounded-full justify-center w-6 h-6 pt-0.5 text-sm ${
              data.isExtended ? "bg-violet-600" : "bg-gray-500"
            }`}
          >
            E
          </div>
        </div>
      </div>
      <div className="flex flex-col ml-4 text-left gap-1">
        <span>0x{data.id.toString(16)}</span>
        <span>{data.length}</span>
        <span>{Math.round(data.frequency * 10) / 10}Hz</span>
      </div>
      <div id="secondColumn" className="flex flex-col flex-1 ml-8">
        {data.signals.map((item, index) => (
          <div key={index} className="flex flex-col overflow-x-auto">
            <div className="flex pr-2">
              <div id="name" className="text-left  py-px  max-w-28 sm:max-w-60 overflow-auto">
                <span className="truncate overflow-hidden whitespace-nowrap block">{item.name}</span>
              </div>
              <div className="flex-1"></div>
              <div className="text-right ml-2">
                <span>{Math.round(item.value * 1000) / 1000}</span>
              </div>
              <div className="text-left ml-2">
                <span>{item.unit}</span>
              </div>
            </div>
            {index < data.signals.length - 1 && <div className="h-[1px] bg-slate-600"></div>}
          </div>
        ))}
      </div>
    </div>
  );
}

export default CanMessageItem;
