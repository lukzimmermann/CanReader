import { CanMessageSignal } from "../../../interfaces/can";

type Props = {
  data: CanMessageSignal;
};

function CanSignalItem({ data }: Props) {
  return (
    <div className="flex hover:bg-slate-600">
      <div className="w-40 text-left">
        <span>{data.name}</span>
      </div>
      <div className="w-14 text-right overflow-clip">
        <span>{data.value}</span>
      </div>
      <div className="w-10 text-left ml-2">
        <span>{data.unit}</span>
      </div>
    </div>
  );
}

export default CanSignalItem;
