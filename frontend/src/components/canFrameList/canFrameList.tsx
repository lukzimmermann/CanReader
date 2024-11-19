import { CanFrame } from "../../interfaces/can";
import CanFrameItem from "./canFrameItem";

type Props = {
  data: CanFrame[];
};

function CanFrameList({ data }: Props) {
  return (
    <div>
      {data.map((item) => (
        <CanFrameItem key={item.id} data={item}></CanFrameItem>
      ))}
    </div>
  );
}

export default CanFrameList;
