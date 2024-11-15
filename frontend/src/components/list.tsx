import { CanData } from "../interfaces/Can";
import ListItem from "./listItem";

type Props = {
  data: CanData[];
};

function List({ data }: Props) {
  return (
    <div>
      {data.map((item) => (
        <ListItem data={item}></ListItem>
      ))}
    </div>
  );
}

export default List;
