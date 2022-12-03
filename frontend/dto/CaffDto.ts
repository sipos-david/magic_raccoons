import CommentDto from "./CommentDto";

export default interface CaffDto {
    id: number,
    month: number,
    day: number,
    year: number,
    hour: number,
    rawfile: string,
    creator: string,
    comments: CommentDto[]
}