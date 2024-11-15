import { CanData } from "../interfaces/Can";

type Props = {
  data: CanData;
};

function ListItem({ data }: Props) {
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
          <span>{data.id}</span>
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
          <span className={binaryDataFieldClass()}>{data.data0.toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data1.toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data2.toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data3.toString(2).padStart(8, "0")}</span>
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
          <span className={binaryDataFieldClass()}>{data.data4.toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data5.toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data6.toString(2).padStart(8, "0")}</span>
          <span className={binaryDataFieldClass()}>{data.data7.toString(2).padStart(8, "0")}</span>
        </div>
      </div>
    </div>
  );
}

export default ListItem;
