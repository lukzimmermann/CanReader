import { CanFrame } from "../../../interfaces/can";
import { DbcCanFrame } from "../../../interfaces/dbc";
import { parseRawCanMessages } from "../../../utils/parseRawCanMessage";
import CanMessageItem from "./canMessageItem";

type Props = {
  data: CanFrame[];
  dbc: DbcCanFrame[];
};

function CanMessageList({ data, dbc }: Props) {
  const x = parseRawCanMessages(data, dbc);

  return (
    <div className="flex flex-col">
      {x.map((e, index) => (
        <CanMessageItem key={index} data={e} />
      ))}
    </div>
  );
}

export default CanMessageList;
