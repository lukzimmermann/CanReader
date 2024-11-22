import { CanFrame } from "../../interfaces/can";

type Props = {
  data: CanFrame;
};

function CanFrameItem({ data }: Props) {
  const binaryDataFieldClass = () => {
    return "font-mono bg-gray-700 rounded px-2";
  };

  return (
    <div className="flex p-3 gap-5 border-t border-t-slate-600 hover:bg-slate-600 justify-between">
      <div id="metaRow" className="flex">
        <div className="flex flex-col text-left gap-1 mr-4">
          <span>ID:</span>
          <span>Length:</span>
          <span>Frequency:</span>
          <div className="flex gap-2">
            <div
              className={`flex rounded-full self-center justify-center pt-0.5 pl-px w-6 h-6 text-sm ${
                data.isExtended ? "bg-violet-600" : "bg-gray-700"
              }`}
            >
              R
            </div>
            <div
              className={`flex rounded-full self-center justify-center pt-0.5 pl-px w-6 h-6 text-sm ${
                data.isExtended ? "bg-violet-600" : "bg-gray-700"
              }`}
            >
              E
            </div>
          </div>
        </div>
        <div className="flex flex-col text-left gap-1 w-24">
          <span>0x{data.id.toString(16).toUpperCase()}</span>
          <span>{data.length}</span>
          <span>{data.frequency}Hz</span>
        </div>
      </div>
      <div id="dataRow1" className="flex text-left gap-1 mr-2">
        <div className="flex flex-col text-left gap-1 mr-2">
          <span>1:</span>
          <span>2:</span>
          <span>3:</span>
          <span>4:</span>
        </div>
        <div className="flex flex-col text-left gap-1">
          <span className={binaryDataFieldClass()}>{data.data[0].toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data[1].toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data[2].toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data[3].toString(2).padStart(8, "0")}</span>
        </div>
      </div>
      <div id="dataRow2" className="flex text-left gap-1 mr-2">
        <div className="flex flex-col text-left gap-1 mr-2">
          <span>5:</span>
          <span>6:</span>
          <span>7:</span>
          <span>8:</span>
        </div>
        <div className="flex flex-col text-left gap-1">
          <span className={binaryDataFieldClass()}>{data.data[4].toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data[5].toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data[6].toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data[7].toString(2).padStart(8, "0")}</span>
        </div>
      </div>
    </div>
  );
}

export default CanFrameItem;
